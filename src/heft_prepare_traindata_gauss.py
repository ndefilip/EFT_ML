import os, sys

# the standard module for tabular data
import pandas as pd

# the standard module for array manipulation
import numpy as np

# the standard modules for high-quality plots
import matplotlib as mp
import matplotlib.pyplot as plt

# update fonts
FONTSIZE = 16
font = {'family' : 'serif',
        'weight' : 'normal',
        'size'   : FONTSIZE}
mp.rc('font', **font)

# set usetex = False if LaTex is not 
# available on your system or if the 
# rendering is too slow
mp.rc('text', usetex=True)

def read_data(datafiles):
    df = []
    for datafile in datafiles:
        print('reading %s' % datafile)
        df.append( pd.read_csv(datafile) )

    # concatenate dataframes
    df = pd.concat(df)
        
    # make number of rows be a multiple of 20
    total  = len(df)
    print(total)
    total  = int(total / 20)
    total  = 20 * total
    
    print('\nnumber of rows read: %d\n' % len(df))

    # randomly shuffle order of rows in dataframe
    df = df.sample(frac=1).reset_index(drop=True)
    return df

datafiles = ['powheg_total_param_gauss_oldStyle.csv']

dfBP = read_data(datafiles)
print(dfBP[:5])



def get_column_names(df, first='17', last='96'):
    # get column names
    columns = list(df.columns)
    params  = columns[0:5]
    
    firstbin= columns.index(first)
    lastbin = columns.index(last)
    
    bins = columns[firstbin:lastbin+1]
    return params, bins

params, bins = get_column_names(dfBP)

params, bins[0], bins[-1], len(bins)

print(params, bins[0], bins[-1], len(bins))

# get Wilson coefficients
wilson = dfBP[params].to_numpy()
print(wilson.shape)
print(wilson)

# get cross section data
BP   = dfBP[bins].to_numpy()
print(BP.shape)

# protect against negative cross sections
BP   = np.where(BP < 0, 0, BP) 

print(BP[0])
print(BP[1])

print(wilson.T[0])

# first place data in a dictionary

dmap = {'klambda': wilson.T[0],
        'CT': wilson.T[1],
        'CTT': wilson.T[2], 
        'CGGH': wilson.T[3], 
        'CGGHH': wilson.T[4]}

dmap.update( {key: BP.T[i] for i, key in enumerate(bins)} )

# then save to a csv file
df = pd.DataFrame(dmap)
df.to_csv('heft_gauss_spectra.csv', index=False)

print(len(df), df[:5])

print("Here I am")

print(BP.shape)
_, xbins = BP.shape

print("Here I am - now")

# compute mid-points of di-Higgs mass bins, with the mass range mapped to the
# unit interval

xmin = 0
xmax = xbins/100. # bin width is 0.01, which corresponds to 15 GeV

print(xbins,xmin,xmax)

x = np.linspace(xmin, xmax, xbins+1) # it divides the 80 bin interval in 81 bins
# x[1:] = x[1], x[2] ...,x[n-1] 
print("x dal bin 1",x[1:]) 
# x[:-1]= x[0], x[1],...,x[n-2]
print("x dal primo zero",x[:-1])

x = (x[1:]+x[:-1])/2

print("Tutto x",x)

# Save data from the last 300 spectra and keep the first 40 spectra to test the spectrum model.
# fill list with training data 
START = 40
data  = []

print("Concatenate numpy arrays")

# print("Wilson coeff.",wilson[START:])
# print("mHH bin contents.",BP[START:])

zipobj=zip(wilson[START:], BP[START:])
print("joining the iterators",tuple(zipobj))

for params, spectrum in zip(wilson[START:], BP[START:]):
    p = list(params)
    # print("wilson params from zip",p)
    # print("mhh spectrum",list(spectrum))

    # increase the incidence of spectra with 
    # cross sections < 0.5 pb
    total_xsec = spectrum.sum()
    if total_xsec < 0.5:
        N = 1
    else:
        N = 1
#    ?????

    d = []
    zipobjnew=zip(x, spectrum)
    print("Joining the new iterators",tuple(zipobjnew))

    for _ in range(N):
        for mhh, sigma in zip(x, spectrum):
            # print("mhh in joined iterator",mhh)
            # print("sigma in joined iterator",sigma)
            d = p + [mhh, sigma] # CTT, CGGH, CGGHH, MHH, SIGMA
            print(d)
            data.append(d)
    
print("Total data are",data) # 300 raws per 80 bin  = 24000

# randomly shuffle rows
data = np.array(data)
np.random.shuffle(data)

print("Total data shuffled are",data) # 300 raws per 80 bin  = 24000
print("Total data shuffled lenght is",len(data))

N = len(data) // 1000
#print(N)
N *= 1000
#print(N)
data = data[:N]
print('training data size:',len(data))

## create dataframe
# data has N=24000 rows x 5 columns
data = data.T  # transpose to shape (5, N) , 5 raws times 24000 columns
print("Transposed data are= ",data.shape)

df = pd.DataFrame({'CTT':   data[0],
                   'CGGH':  data[1],
                   'CGGHH': data[2],
                   'mhh':   data[3],
                   'sigma': data[4]})

# save training data to a csv file
df.to_csv('heft_gauss_traindata.csv', index=False)

print(df)
