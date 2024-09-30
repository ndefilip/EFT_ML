#!/usr/bin/env python
#-----------------------------------------------------------------------------
import os, sys, re
import numpy as np
import pandas as pd
import turtlebinning as tb

from array import array
#-----------------------------------------------------------------------------
def generate_data(npoints=100000):
    # generate data and create a dataframe
    x  = np.random.normal(loc=0, scale=1, size=npoints)
    y  = np.random.normal(loc=0, scale=2, size=npoints)
    z  = np.random.normal(loc=0, scale=3, size=npoints)
    df = pd.DataFrame({'x': x, 'y': y, 'z': z})
    return df
#-----------------------------------------------------------------------------
def main():

    # generate data and concatenate two data columns
    # into a single non-numpy array
    
    nbins   = 1000       # number of bins
    npoints_per_bin = 7  # number of points per bin
    npoints = nbins * npoints_per_bin
    
    df   = generate_data(npoints=npoints)
    data = array('d')
    data.extend(df['x'])
    data.extend(df['y'])

    # bin data
    npoints = len(df) 
    nparams = 2        # number of parameters
    ttb     = tb.Turtle(data, nbins, npoints, nparams)

    # loop over first K points
    params  = ['x', 'y']
    K = 10
    for ii in range(K):

        point = array('d', df[params].iloc[ii])
    
        # find bin in which this point resides ...

        ibin = ttb.find(point)
        if ibin < 0:
            raise ValueError(f'point {str(point):s} not in any bin')
        else:
            indices = ttb.indices(ibin)

            # ... and print the indices of all points in that bin
            print(f'point {ii:2d} in bin {ibin:4d} containing points: ', end='')
            ll = 0
            for jj in indices:
                print(f'{jj:5d}', end='')
                ll += 1
                if ll >= 10:
                    print()
            if ll < 10:
                print()
#----------------------------------------------------------------------------
main()
