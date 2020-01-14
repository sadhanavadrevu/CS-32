#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome& genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const;
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const;
private:
    
    struct DNAsubsequence {
        int genomePosition;
        int position;
    };
    
    int matchLength(string toCheck, string toMatch, bool exactMatchOnly) const {
        int mismatch = 0;
        int length = 0;
        for (int i = 0; i < toCheck.size(); i++) {
            if (toCheck[i] != toMatch[i]) {
                mismatch++;
            }
            if (exactMatchOnly && mismatch > 0)
                return length;
            else if (!exactMatchOnly && mismatch > 1)
                return length;
            length++;
        }
        return length;
    }
    
    int m_minLength;
    vector<Genome> genomes;
    Trie<DNAsubsequence> DNAmap;
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
{
    m_minLength = minSearchLength;
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
    genomes.push_back(genome);
    int i = 0;
    while (i + m_minLength <= genome.length()) {
        string s;
        genome.extract(i, m_minLength, s);
        DNAsubsequence seq;
        seq.genomePosition = (int)genomes.size() - 1;
        seq.position = i;
        DNAmap.insert(s, seq);
        i++;
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    vector<DNAMatch> result;
    if (fragment.size() < minimumLength)
        return false;
    if (minimumLength < m_minLength)
        return false;
    string key = fragment;
    string s = key.substr(0, m_minLength); //pre-fix to search for
    vector<DNAsubsequence> found = DNAmap.find(s, exactMatchOnly); //find genomes that have matches for the pre-fix
    for (int i = 0; i < found.size(); i++) { //for every match found (number of distinct hits across all genomes)
        string extracted;
        int genomePos = found[i].genomePosition;
        genomes[genomePos].extract(found[i].position, (int)fragment.size(), extracted); //longest possible match is as long fragment size
        int length = matchLength(extracted, fragment, exactMatchOnly); //check how much of longest possible match is actually a match
        if (length < minimumLength) //if match isn't long enough, move on to next match
            continue;
        bool genomeFound = false;
        for (int k = 0; k < result.size(); k++) {
            if (result[k].genomeName == genomes[genomePos].name()) { //check if genome already has a recorded match
                genomeFound = true;
                if (result[k].length < length) { //if this match is longer than than the existing match for this genome, replace it
                    result[k].position = found[i].position;
                    result[k].length = length;
                    break;
                }
            }
        }
        if (genomeFound == false) { //if genome does not already have a recorded match, create one
            DNAMatch newMatch;
            newMatch.genomeName = genomes[found[i].genomePosition].name();
            newMatch.length = length;
            newMatch.position = found[i].position;
            result.push_back(newMatch);
        }
    }
    matches = result;
    if (matches.empty())
        return false;
    return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    if (fragmentMatchLength < m_minLength)
        return false;
    vector<GenomeMatch> toReturn;
    int numFrags = query.length()/fragmentMatchLength;
    vector<int> numMatches(genomes.size(), 0);
    for (int i = 0, pos = 0; i < numFrags; i++, pos += fragmentMatchLength) {
        string s;
        query.extract(pos, fragmentMatchLength, s);
        vector<DNAMatch> matchesFound;
        if (findGenomesWithThisDNA(s, fragmentMatchLength, exactMatchOnly, matchesFound)) {
            for (int k = 0; k < matchesFound.size(); k++) {
                for (int j = 0; j < genomes.size(); j++) {
                    if (matchesFound[k].genomeName == genomes[j].name()) {
                        numMatches[j]++;
                    }
                }
            }
        }
    }
    for (int i = 0; i < numMatches.size(); i++) {
        if (numMatches[i] != 0) {
            double matchPerc = (double)numMatches[i]/numFrags * 100;
            if (matchPerc > matchPercentThreshold) {
                GenomeMatch newMatch;
                newMatch.genomeName = genomes[i].name();
                newMatch.percentMatch = matchPerc;
                toReturn.push_back(newMatch);
            }
        }
    }
    if (toReturn.size() == 0)
        return false;
    results = toReturn;
    return true;
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
