#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <cctype>
using namespace std;

class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence);
    static bool load(istream& genomeSource, vector<Genome>& genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string& fragment) const;
private:
    string m_name;
    string m_sequence;
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence)
{
    m_name = nm;
    m_sequence = sequence;
}

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    vector<Genome> result;
    
    string s;
    getline(genomeSource, s);
    while (s.size() != 0) {
        string name;
        if (s.size() > 1 && s[0] == '>')
            name = s.substr(1);
        else
            return false;
        
        string sequence;
        getline(genomeSource, s); //get next line in string
        if (s.size() == 0) {
            return false;
        }
        
        while (s[0] != '>' && s.size() != 0) {
            for (int i = 0; i < s.size(); i++) {
                toupper(s[i]);
                if (s[i] != 'A' && s[i] != 'C' && s[i] != 'T' && s[i] != 'G' && s[i] != 'N') {
                    return false;
                }
                sequence += s[i];
            }
            getline(genomeSource, s);
        }
        Genome toAdd(name, sequence);
        result.push_back(toAdd);
    }
    genomes = result;
    return true;
}

int GenomeImpl::length() const
{
    return (int)m_sequence.size();
}

string GenomeImpl::name() const
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const {
    if (position + length > m_sequence.size())
        return false;
    fragment = m_sequence.substr(position, length);
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes) 
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
