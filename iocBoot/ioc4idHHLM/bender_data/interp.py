#!/usr/bin/env python3
'''
Generates concave.csv and convex.csv
with interpolated values between the measured values
in bender.csv
'''
import numpy as np
from matplotlib import pyplot as plt
from scipy.interpolate import interp1d

N_POINTS = 100 # total number of interpolated values to generate
SAVE = False   # save interpolated data to CSV

# load the raw data and plot it
data = np.loadtxt("./bender.csv", delimiter=",")
index = data[:,0]
fig0, ax0 = plt.subplots()
ax0.plot(data[:,1][0:7], data[:,3][0:7], "bo")
ax0.plot(data[:,1][8:], data[:,3][8:], "bo")
ax0.plot(data[7][1],data[7][3], "bo")
ax0.set(xlabel="Upstream encoder counts", ylabel="Radius of Curvature (km)")
ax0.grid()

# Concave (positive ROC)
ucounts_concave = data[:,1][0:7]
ucounts_concave = ucounts_concave - data[7][1]
roc_concave = data[:,3][0:7]
f_concave = interp1d(ucounts_concave, roc_concave, kind="cubic")
x_fit_concave = np.linspace(ucounts_concave[0], ucounts_concave[-1], N_POINTS)
y_fit_concave = [f_concave(xi) for xi in x_fit_concave]
fig1, ax1 = plt.subplots()
ax1.plot(ucounts_concave, roc_concave, "bo", label="measured")
ax1.plot(x_fit_concave, y_fit_concave, "r.", label="interpolated")
ax1.set(title="Encoder counts (normalized) vs. Concave RoC")
ax1.set(xlabel="Encoder counts")
ax1.set(ylabel="Radius of curvature (km)")
ax1.set_yticks(np.arange(0,150,10))
ax1.legend()
ax1.grid()

# Convex (negative ROC)
ucounts_convex = data[:,1][8:]
ucounts_convex = ucounts_convex - data[7][1]
roc_convex = data[:,3][8:]
f_convex = interp1d(ucounts_convex, roc_convex, kind="cubic")
x_fit_convex = np.linspace(ucounts_convex[0], ucounts_convex[-1], N_POINTS)
y_fit_convex = [f_convex(xi) for xi in x_fit_convex]
fig2, ax2 = plt.subplots()
ax2.plot(ucounts_convex, roc_convex, "bo", label="measured")
ax2.plot(x_fit_convex, y_fit_convex, "r.", label="interpolated")
ax2.set(title="Encoder Counts (normalized) vs. Convex RoC")
ax2.set(xlabel="Encoder counts")
ax2.set(ylabel="Radius of curvature (km)")
ax2.set_yticks(np.arange(-100,30,10))
ax2.legend()
ax2.grid()

# save the interpolated data to CSV files
if SAVE:
    np.savetxt("concave.csv", np.column_stack([x_fit_concave, y_fit_concave]), delimiter=",")
    np.savetxt("convex.csv", np.column_stack([x_fit_convex, y_fit_convex]), delimiter=",")

# show the plots
plt.show()

