#!/usr/bin/env python

import numpy as np

def getLatDynMat(u):
    '''
    u has to be greater than 0.1, other otherwise the numerical integration
    can brow up
    '''
    # if (u < 0.1):
    #     raise Exception('Longitudinal velocity less than 0.1!!!')

    numiter = 100
    dt = 0.1
    a = 0.17
    b = 0.16
    Caf = 14.66
    Car = Caf*a/b
    Iz = 0.03332

    A = np.array([[0, 1, u, 0],
        [0, -(Caf + Car)/u, 0, ((-a*Caf + b*Car)/u - u)],
        [0, 0, 0, 1],
        [0, (-a*Caf + b*Car)/(Iz*u), 0, -(a**2 *Caf + b**2 *Car)/(Iz*u)]])
    B = np.transpose(np.array([0, Caf, 0, a*Caf/Iz]))
    E = np.transpose(np.array([0, 0, -1, 0]))
    K = np.transpose(np.array([0, 0, 0, 0]))
    At = np.eye(4) * dt
    integral_A = At
    Ad = np.eye(4)
    for i in range(2, numiter):
        At = np.dot(At, A*dt)/i
        integral_A += At
        Ad += At/dt*i

    Ad = tuple(np.reshape(Ad.T, (1, -1))[0])
    Bd = tuple(np.dot(integral_A, B))
    Ed = tuple(np.dot(integral_A, E))
    return Ad, Bd, Ed

