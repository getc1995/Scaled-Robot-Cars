/* Produced by CVXGEN, 2018-02-12 03:46:40 -0500.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */
#include <ros/ros.h>
#include <um_acc/MPC_CC.h>
#include "ccsolver.h"
Vars vars;
Params params;
Workspace work;
Settings settings;
bool compute(um_acc::MPC_CC::Request &req, um_acc::MPC_CC::Response &res);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "MPC_CC_server2");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("MPC_CC2", compute);
    ROS_INFO("MPC CC ready.");
    ros::spin();

    return 0;
}

bool compute(um_acc::MPC_CC::Request &req, um_acc::MPC_CC::Response &res) {
    set_defaults();
    setup_indexing();

    params.Vr[0] = req.vr;    
    params.x_0[0] = req.u0;
    params.u_0[0] = req.i0;
    /* Make this a diagonal PSD matrix, even though it's not diagonal. */
    params.Q[0] = req.wv;// Wv
    params.R[0] = req.wi;// Wi

    params.A[0] = 0.9994;
    params.B[0] = 0.0289;
    params.F[0] = -0.0956;

    if (req.u0 < 0)
        params.F[0] = -params.F[0];

    params.u_max[0] = req.i_max;
    params.u_min[0] = req.i_min;

    /* Solve problem instance for the record. */
    settings.verbose = 0;
    solve();
    // int cntprint = 1;
    // for (; cntprint < 21; cntprint++){
    //     printf("x: %.3f, u: %.3f.\n", *vars.x[cntprint], *vars.u[cntprint]);
    // }
    res.i = *vars.u[1];
    return work.converged;
}