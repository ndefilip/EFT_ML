# turtlebinning

## Introduction
This is a wrapper around the C++ class __TKDTreeBinning__ from the CERN data-analysis package [ROOT](https://root.cern.ch). TKDTreeBinning bins n-dimensional data using recursive binary partitioning. At
each step, the algorithm splits every bin into two bins with equal numbers of
entries in
each. The partitioning continues until the specified number of bins is
reached.  For example, if you have 1,000,000 points in a
10-dimensional space and ask for 1,000 bins in that space then the
number of points per bin will be 1,000,000/1000 = 1,000 and the amount
of space allocated for the data would be 10,000,000. The volume of bins will typically vary across the space. The density at the center of each bin is approximated by the number of points per bin / bin volume.


## Dependencies
This package, which is written in C++ and which can be used with Python, depends on [ROOT](https://root.cern.ch) and the C++ Standard Template Library. It has been tested successfully with ROOT version 6.26/10 and Python version 3.11.0. The C++ in the source code (Turtle.cc, Turtle.h) is hardly cutting edge so it ought to compile with any standard compiler, such as clang++ or g++, and with versions of ROOT that are relatively recent, say later than version 6.18.00. The Python binding is based on PyROOT.

## Installation
Download the code using:
```bash
git clone https://github.com/hbprosper/turtle
cd turtle
make
```
If you have the software environment system [Miniconda](https://docs.conda.io/en/latest/miniconda.html) installed, which is recommended, you can install the Python module __turtlebinning__ in your current miniconda environment with
```bash
make install
```
and uninstall it with
```bash
make uninstall
```
Otherwise, source __setup.sh__ to define a few environment variables. If you are running a shell other than bash, you will have to make a version of setup.sh for your shell. The Python module is called turtlebinning to avoid conflict with the module turtle.

__turtlebinning__ can work directly on flat ROOT files (think of them as binary versions of csv files, i.e., 
tables). It also works directly on an appropriately structured
array as in the example below. This is convenient if you are using, for example, pandas dataframes.

## Example
Suppose you have a __pandas__ dataframe, __df__, and you wish to bin
points defined by the variables *x* , *y*, and *z*. Here are the steps
assuming you already have access to __df__. __Note__: the length of
each sequence df['x'] etc. must be the same, and is assumed to be the same.

### Step 1

Import the relevant modules.
```python
import turtlebinning as tt
from array import array
```

### Step 2
Create an array and fill it as follows.
```python
data  = array('d')
data.extend(df['x'])
data.extend(df['y'])
data.extend(df['z'])
```

### Step 3
Bin the data.
```python
nbins    = 1000          # number of bins
npoints  = len(df['x'])  # number of points
nparams  = 3             # number of dimensions
ttb      = tt.Turtle(data, nbins, npoints, nparams)
```

You can find in which bin the point $(x_0, y_0, z_0)$ lies using
```python
point = array('d', [x0, y0, z0])
ibin   = ttb.findBin(point)
```

Then you can find the indices of the points that lie in bin $ibin$
using
```python
ind = ttb.indices(ibin)
```
This returns an object of type __vector\<int\>__ containing the
indices (that is, the ordinal values of the points in the dataframe
__df__) of the points that lie in bin $ibin$.  The STL vector class
works as expected. See __test/testturtle.py__ for a more detailed example.
