#ifndef TURTLE_H
#define TURTLE_H
// ---------------------------------------------------------------------------
// Created May 11, 2011 by Harrison Prosper and Sezen Sekmen
// Updated May 21, 2015 HBP - Implement Fill
//         May 18, 2019 HBP - add FindBin method
// ---------------------------------------------------------------------------
#include <vector>
#include <string>
#include <map>
#include "TKDTreeBinning.h"
// ---------------------------------------------------------------------------
///
class Turtle
{
public:
  ///
  Turtle();

  ///
  Turtle(std::string rootfilename, 
	 std::vector<std::string>& variablenames, 
	 std::string treename,
	 int numberofbins,
	 int numberofpoints=-1);
  
  ///
  Turtle(std::vector<std::string>& rootfilenames, 
	 std::vector<std::string>& variablenames, 
	 std::string treename,
	 int numberofbins,
	 int numberofpoints=-1);

  ///
  Turtle(double* data,
  	 int numberofbins,
  	 int numberofpoints,
  	 int numberofvariables);

  ///
  void build(std::string rootfilename, 
	     std::vector<std::string>& variablenames, 
	     std::string treename,
	     int numberofbins,
	     int numberofpoints=-1);
  
  ///
  void build(std::vector<std::string>& rootfilenames, 
	     std::vector<std::string>& variablenames, 
	     std::string treename,
	     int numberofbins,
	     int numberofpoints=-1);

  virtual ~Turtle();
  
  ///
  double density(int bin) { return _btree->GetBinDensity(bin); }
  
  ///
  size_t binMinDensity() { return _btree->GetBinMinDensity(); }

  ///
  size_t binMaxDensity() { return _btree->GetBinMaxDensity(); }
  
  ///
  double volume(int bin)  { return _btree->GetBinVolume(bin); }

  ///
  const double* center(int bin) { return _btree->GetBinCenter(bin); }

  ///
  const double* width(int bin)  { return _btree->GetBinWidth(bin); }

  ///
  const double* minEdges(int bin)  { return _btree->GetBinMinEdges(bin); }

  ///
  const double* maxEdges(int bin)  { return _btree->GetBinMaxEdges(bin); }

  ///
  void sortDensity(bool ascend=false) { _btree->SortBinsByDensity(ascend); }

  ///
  size_t findBin(std::vector<double>& point)
  { return _btree->FindBin(&point[0]); }

  ///
  size_t find(std::vector<double>& point)
  { return _btree->FindBin(&point[0]); }

  ///
  size_t findBin(double* point)
  { return _btree->FindBin(point); }

  ///
  size_t find(double* point)
  { return _btree->FindBin(point); }


  size_t nBins() { return _btree->GetNBins();  }
  
  size_t entriesPerBin() { return _entries_per_bin; }
  
  ///
  std::vector<std::vector<double> >  pointsInBin(int bin)
    { return _btree->GetPointsInBin( bin ); }

  ///
  std::vector<std::vector<double> >  points(int bin)
    { return _btree->GetPointsInBin( bin ); }

  
  /// Return indices of points in given bin.
  std::vector<int>  indices(int bin);

  /// Clear counts and variances
  void clear();
  
  /// Histogram data from specified file.
  void fill(std::string rootfilename,
	    std::string weightname="");

    /// Histogram data from specified files.
  void fill(std::vector<std::string>& rootfilenames,
	    std::string weightname="");

  /// Histogram data.
  void fill(std::vector<double>& point, double weight=1);

  /// Histogram data.
  void fill(double* point, double weight=1);
  
  /// Return bin counts for histogrammed data.
  std::vector<double> counts() { return _counts; }

  /// Return bin variances for histogrammed data.
  std::vector<double> lowEdges() { return _variances; }

  
  ClassDef(Turtle,0)
  
 private:
  TKDTreeBinning*          _btree;
  std::vector<std::string> _rootfilenames;
  std::vector<std::string> _variablenames; 
  std::string              _treename;
  std::vector<double>      _counts;
  std::vector<double>      _variances;
  std::map<int, std::vector<int> > _indicesmap;
  
  size_t  _numberofbins;
  size_t  _entries_per_bin;
  size_t  _datasize;
  double* _data;
  double* _point;
  size_t  _numberofvars;
  
  double* _readTree(std::vector<std::string>& rootfilenames, 
                    std::vector<std::string>& variablenames, 
                    std::string treename,
                    int numberofbins,
                    int numberofpoints);

  /// Build map from bin number to the indices of the points within the bin
  void _buildIndicesMap();
};

#endif
