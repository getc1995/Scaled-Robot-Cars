/* Produced by CVXGEN, 2018-02-12 21:04:55 -0500.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: testsolver.c. */
/* Description: Basic test harness for solver.c. */
#include "accsolver.h"
#include <um_acc/MPC_ACC.h>
#include <ros/ros.h>
Vars vars;
Params params;
Workspace work;
Settings settings;
bool compute(um_acc::MPC_ACC::Request &req, um_acc::MPC_ACC::Response &res);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "MPC_ACC_server2");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("MPC_ACC2", compute);
    ROS_INFO("MPC ACC ready.");
    ros::spin();

    return 0;
}

bool compute(um_acc::MPC_ACC::Request &req, um_acc::MPC_ACC::Response &res) {
    set_defaults();
    setup_indexing();
    /* Make this a diagonal PSD matrix, even though it's not diagonal. */
    params.Q[0] = req.wv; // Wv
    params.Q[1] = 0;
    params.Q[2] = 0;
    params.Q[3] = req.wh; // Wh
    /* Make this a diagonal PSD matrix, even though it's not diagonal. */
    params.R[0] = req.wi; // Wi Penalty on the rate

    params.x_0[0] = req.u0;
    params.x_0[1] = req.h0;
    params.vl[0] = req.vl;
    params.u_0[0] = req.i0;

    /*Dynamics matrix*/
    params.A[0] = 0.9942;
    params.A[1] = -0.0997;
    params.A[2] = 0;
    params.A[3] = 1;

    params.B[0] = 0.0288;
    params.B[1] = -0.0014;
    params.E[0] = 0;
    params.E[1] = 0.1;

    params.F[0] = -0.0654;
    params.F[1] = 0.0033;

    params.h_stop[0] = req.h_stop;
    params.T_gap[0] = req.T_gap;
    params.v_max[0] = req.v_max;
    params.v_min[0] = req.v_min;
    params.h_min[0] = req.h_min;
    params.u_max[0] = req.i_max;
    params.u_min[0] = req.i_min;
    /* Solve problem instance for the record. */
    settings.verbose = 0;
    solve();
    res.i = *vars.u[1];
    return work.converged;
}
