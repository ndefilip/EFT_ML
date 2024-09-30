// ---------------------------------------------------------------------------
// File: Turtle.cc
// Description: Bin data in n-dimensions using bins created by recursive
// binary partitioning, such that the count in each bin is the same.
// Created May 11, 2011 by Harrison Prosper and Sezen Sekmen
// Updated May 21, 2015 HBP - Implement Fill
// Updated Mar 10, 2023 HBP - Add constructor that takes an data array
//                            add indices(bin) method
// ---------------------------------------------------------------------------
#include <algorithm>
#include <iostream>
#include <cassert>
#include "TMath.h"
#include "TChain.h"
#include "Turtle.h"
// ---------------------------------------------------------------------------

using namespace std;

Turtle::Turtle()
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _indicesmap(map<int, vector<int> >()),
    _datasize(0),
    _data(0),
    _point(0)
{
}

Turtle::Turtle(string rootfilename,
               vector<string>& variablenames,
               string treename,
               int numberofbins,
               int numberofpoints)
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _indicesmap(map<int, vector<int> >()),
    _datasize(0),
    _data(0),
    _point(0)
{
  build(rootfilename,
	variablenames,
	treename, 
	numberofbins,
	numberofpoints);
}


Turtle::Turtle(vector<string>& rootfilenames,
               vector<string>& variablenames,
               string treename,
               int numberofbins,
               int numberofpoints)
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _indicesmap(map<int, vector<int> >()),
    _datasize(0),
    _data(0),
    _point(0)
{
  build(rootfilenames,
	variablenames,
	treename, 
	numberofbins,
	numberofpoints);
}


Turtle::Turtle(double* data,
	       int numberofbins,
               int numberofpoints,
	       int numberofvariables)
  : _btree(0),
    _rootfilenames(vector<string>()),
    _variablenames(vector<string>()),
    _treename(""),
    _counts(vector<double>()),
    _variances(vector<double>()),
    _indicesmap(map<int, vector<int> >()),
    _datasize(0),
    _data(0),
    _point(0)
{
  _numberofbins    = numberofbins;
  _entries_per_bin = numberofpoints / _numberofbins;
  _datasize        = _entries_per_bin * _numberofbins;
  
  // make sure datasize matches numberofpoints
  assert ( _datasize == numberofpoints );
    
  _numberofvars    = numberofvariables;
  _counts          = vector<double>(numberofbins, 0);
  _variances       = vector<double>(numberofbins, 0);
  
  // Allocate enough space for the number of points times the 
  // number of variables
  int buffersize = _datasize * numberofvariables;
  _data  = new double[buffersize];
  copy(data, data+buffersize, _data);

  // Allocate space for a single point
  _point = new double[_numberofvars];

  _btree = new TKDTreeBinning(_datasize,
			      _numberofvars, 
			      _data, 
			      _numberofbins);
  _buildIndicesMap();
  
  cout << "number of bins: " << _numberofbins << endl;
  cout << "entries/bin:    " << _entries_per_bin << endl;
  cout << "data size:      " << _datasize << endl;
}


Turtle::~Turtle()
{
  if ( _btree ) delete _btree;
  if ( _data )  delete _data;
  if ( _point)  delete _point;
}

void Turtle::build(vector<string>& rootfilenames,
		   vector<string>& variablenames,
		   string treename,
		   int numberofbins,
		   int numberofpoints)
{
  _rootfilenames = rootfilenames;
  _variablenames = variablenames;
  _treename      = treename;
  _counts        = vector<double>(numberofbins, 0);
  _variances     = vector<double>(numberofbins, 0);

  double* data = _readTree(rootfilenames,
			   variablenames,
			   treename, 
			   numberofbins,
			   numberofpoints);
  
 _btree = new TKDTreeBinning(_datasize,
			     variablenames.size(), 
			     data, 
			     numberofbins);

  _buildIndicesMap();
  
  _numberofbins = _btree->GetNBins();
}

void Turtle::build(string rootfilename,
		   vector<string>& variablenames,
		   string treename,
		   int numberofbins,
		   int numberofpoints)
{
  vector<string> rootfilenames(1, rootfilename);
  build(rootfilenames, 
	variablenames,
	treename, 
	numberofbins,
	numberofpoints);
}


namespace  {
  double zero(double) { return 0; }
};

void Turtle::fill(string rootfilename, string weightname)
{
  vector<string> rootfilenames(1, rootfilename);
  fill(rootfilenames, weightname);
}

void Turtle::fill(std::vector<std::string>& rootfilenames,
		  std::string weightname)
{
  assert( _btree );
  assert( (size_t)_btree->GetNBins() == _counts.size() );
  
  // Histogram data from given files and store values in _counts
  TChain chain(_treename.c_str());
  for (size_t i=0; i < rootfilenames.size(); i++)
    chain.Add(rootfilenames[i].c_str());

  int numberofpoints = chain.GetEntries();

  // create a branch for each variable
  for (size_t i=0; i < _variablenames.size(); i++)
    chain.SetBranchAddress(_variablenames[i].c_str(), &_point[i]);

  double weight = 1.0;
  if ( weightname != "" )
    chain.SetBranchAddress(weightname.c_str(), &weight);
  
  // clear _counts, _variances
  clear();

  for (int entry=0; entry < numberofpoints; entry++)
    {
      chain.GetEntry(entry);
      if ( entry % 25000 == 0 ) 
        cout << entry << endl;
      int bin = _btree->FindBin(_point);
        if ( bin < 0 ) continue;
	if ( (size_t)bin >= _counts.size() ) continue;
      _counts[bin] += weight;
      _variances[bin] += weight*weight;
    }
}

void Turtle::fill(std::vector<double>& point, double weight)
{
  // Histogram data and store values in _counts
  assert( _btree );
  assert( (size_t)_btree->GetNBins() == _counts.size() );
  int bin = _btree->FindBin(&point[0]);
  if ( bin < 0 ) return;
  if ( (size_t)bin >= _counts.size() ) return;
  _counts[bin] += weight;
  _variances[bin] += weight*weight;
}


void Turtle::fill(double* point, double weight)
{
  // Histogram data and store values in _counts
  assert( _btree );
  assert( (size_t)_btree->GetNBins() == _counts.size() );
  int bin = _btree->FindBin(point);
  if ( bin < 0 ) return;
  if ( (size_t)bin >= _counts.size() ) return;
  _counts[bin] += weight;
  _variances[bin] += weight*weight;
}


void Turtle::clear()
{
  // clear _counts
  transform(_counts.begin(), _counts.end(), _counts.begin(), zero);
  transform(_variances.begin(), _variances.end(), _variances.begin(), zero);
}

double* Turtle::_readTree(vector<string>& rootfilenames, 
                          vector<string>& variablenames, 
                          string treename, 
                          int numberofbins,
                          int numberofpoints)
{
  TChain chain(treename.c_str());
  for (size_t i=0; i<rootfilenames.size(); i++)
    chain.Add(rootfilenames[i].c_str());

  _numberofbins =  numberofbins;
  _datasize = chain.GetEntries();
  if (numberofpoints > 0)
    _datasize = TMath::Min(_datasize, (size_t)numberofpoints); 
  _entries_per_bin = _datasize / _numberofbins;
  _datasize  = _entries_per_bin * _numberofbins;
  
  // Allocate enough space for the number of points times the 
  // number of variables
  _numberofvars  = variablenames.size();
  int buffersize = _datasize * _numberofvars;
  _data = new double[buffersize];

  // Allocate space for a single point
  _point  = new double[_numberofvars];

  for (size_t i=0; i < variablenames.size(); i++)
    chain.SetBranchAddress(variablenames[i].c_str(), &_point[i]);

  for (size_t entry=0; entry < _datasize; entry++)
    {
      chain.GetEntry(entry);
      if ( entry % 25000 == 0 ) 
        cout << entry << endl;

      for (size_t j=0; j< variablenames.size(); j++)
        _data[entry+j*_datasize] = _point[j];
    }
  cout << "number of bins: " << _numberofbins << endl;
  cout << "entries/bin:    " << _entries_per_bin << endl;
  cout << "data size:      " << _datasize << endl;
  return &_data[0];
}


/// Build map from bin number to the indices of the points within the bin
void Turtle::_buildIndicesMap()
{
  cout << "building indices map..." << endl;
  // initialize map
  for (size_t bin=0; bin < _numberofbins; bin++)
    {
      _indicesmap[bin] = vector<int>();
    }
  
  // loop over points
  // 1. get coordinates of point
  // 2. find bin into which point falls
  // 3. add index of point to bin to index map
  for (size_t entry=0; entry < _datasize; entry++)
    {
      if ( entry % 50000 == 0 )
	{
	  char str[80];
	  printf("  %10d\n", (int)entry); 
	}
      
      // 1. get point
      for (size_t j=0; j < _numberofvars; j++)
        _point[j] = _data[j*_datasize + entry];
      
      // 2. find bin
      size_t bin = _btree->FindBin(_point);
      assert(bin >= (size_t)0);
	
      _indicesmap[bin].push_back(entry);
    }
  cout << "done" << endl;
}

std::vector<int>  Turtle::indices(int bin)
{
  if ( _indicesmap.find(bin) != _indicesmap.end() )
    {
      return _indicesmap[bin];
    }
  else
    {
      return vector<int>();
    } 
}
