/* Produced by CVXGEN, 2018-02-12 23:01:27 -0500.  */
/* CVXGEN is Copyright (C) 2006-2017 Jacob Mattingley, jem@cvxgen.com. */
/* The code in this file is Copyright (C) 2006-2017 Jacob Mattingley. */
/* CVXGEN, or solvers produced by CVXGEN, cannot be used for commercial */
/* applications without prior written permission from Jacob Mattingley. */

/* Filename: testsolver.c. */
/* Description: Basic test harness for solver.c. */
#include <ros/ros.h>
#include "lksolver.h"
#include <um_acc/MPC_LK.h> // srv file
#include <iostream>
#ifndef pi
#define pi 3.1415926
#endif
Vars vars;
Params params;
Workspace work;
Settings settings;
bool compute(um_acc::MPC_LK::Request &req, um_acc::MPC_LK::Response &res);

int main(int argc, char **argv)
{
    ros::init(argc, argv, "MPC_LK_server1");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("MPC_LK1", compute);
    ROS_INFO("MPC LK ready.");
    ros::spin();

    return 0;
}

bool compute(um_acc::MPC_LK::Request &req, um_acc::MPC_LK::Response &res) {
    std::cout << "MPC start computing..." << std::endl;
    set_defaults();
    setup_indexing();
    params.Wy[0] = req.wy;
    params.Wv[0] = req.wv;
    params.Wp[0] = req.wp;
    params.Wr[0] = req.wr;
    params.Ws[0] = req.ws;

    params.y_max[0] = req.y_max;
    params.y_min[0] = req.y_min;
    params.v_max[0] = req.v_max;
    params.p_max[0] = req.p_max;
    params.r_max[0] = req.r_max;
    params.s_max[0] = req.s_max;

    params.x_0[0] = req.y0;
    params.x_0[1] = req.v0;
    params.x_0[2] = req.p0;
    params.x_0[3] = req.r0;
    params.s_0[0] = req.s0;

    /*Index is sequenced by the column*/
    params.A[0] = req.A[0];
    params.A[1] = req.A[1];
    params.A[2] = req.A[2];
    params.A[3] = req.A[3];
    params.A[4] = req.A[4];
    params.A[5] = req.A[5];
    params.A[6] = req.A[6];
    params.A[7] = req.A[7];
    params.A[8] = req.A[8];
    params.A[9] = req.A[9];
    params.A[10] = req.A[10];
    params.A[11] = req.A[11];
    params.A[12] = req.A[12];
    params.A[13] = req.A[13];
    params.A[14] = req.A[14];
    params.A[15] = req.A[15];

    params.B[0] = req.B[0];
    params.B[1] = req.B[1];
    params.B[2] = req.B[2];
    params.B[3] = req.B[3];
    params.E[0] = req.E[0];
    params.E[1] = req.E[1];
    params.E[2] = req.E[2];
    params.E[3] = req.E[3];

    params.rd_0[0] = req.rd[0];
    params.rd_1[0] = req.rd[1];
    params.rd_2[0] = req.rd[2];
    params.rd_3[0] = req.rd[3];
    params.rd_4[0] = req.rd[4];
    params.rd_5[0] = req.rd[5];
    params.rd_6[0] = req.rd[6];
    params.rd_7[0] = req.rd[7];
    params.rd_8[0] = req.rd[8];
    params.rd_9[0] = req.rd[9];
    params.rd_10[0] = req.rd[10];
    params.rd_11[0] = req.rd[11];
    params.rd_12[0] = req.rd[12];
    params.rd_13[0] = req.rd[13];
    params.rd_14[0] = req.rd[14];
    params.rd_15[0] = req.rd[15];
    params.rd_16[0] = req.rd[16];
    params.rd_17[0] = req.rd[17];
    params.rd_18[0] = req.rd[18];
    params.rd_19[0] = req.rd[19];
    params.rd_20[0] = req.rd[20];
    /* Solve problem instance for the record. */
    settings.verbose = 0;
    solve();
    res.s = *vars.s[1];
    std::cout << "Result: " << res.s << std::endl;
    return work.converged;
}
