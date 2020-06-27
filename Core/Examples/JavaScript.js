
// This file is part of Rts, the software framework developed in the research group of 
// Professor Terje Haukaas at the University of British Columbia in Vancouver, Canada
// Web: terje.civil.ubc.ca  Email: terje@civil.ubc.ca



// Compute the responses u1 and u2 from subroutines that are defined below
u1 = computeU1(x1, x2, x3);
u2 = computeU2(x1, x2, x3);

// Print the responses to Rt's Output Pane
print("u1 = ", u1);
print("u2 = ", u2);

// Evaluate the gradients by DDM, if the user asks for it in the function specification
if (evaluateDDM) {
    var u1Gradient = computeU1Gradient(x1, x2, x3);
    d_u1_d_x1 = u1Gradient[0];
    d_u1_d_x2 =  u1Gradient[1];
    d_u1_d_x3 =  u1Gradient[2];
    
    var u2Gradient = computeU2Gradient(x1, x2, x3);
    d_u2_d_x1 = u2Gradient[0];
    d_u2_d_x2 =  u2Gradient[1];
    d_u2_d_x3 =  u2Gradient[2];
}



// Subroutines to evaluate responses and their gradients

function computeU1(x1, x2, x3) {
    var u1 = x2 / (1000 * x3);
    
    return u1;
}

function computeU2(x1, x2, x3) {
    var u2 = Math.pow(x1 / (200 * x3), 2);
    
    return u2;
}

function computeU1Gradient(x1, x2, x3) {
    var d_u1_d_x1 = 0;
    var d_u1_d_x2 = 1 / (1000 * x3);
    var d_u1_d_x3 = -x2 / (1000 * Math.pow(x3, 2));
    
    return [d_u1_d_x1, d_u1_d_x2, d_u1_d_x3];
}

function computeU2Gradient(x1, x2, x3) {
    var d_u2_d_x1 = 2 * x1 / Math.pow(200 * x3, 2);
    var d_u2_d_x2 = 0;
    var d_u2_d_x3 = -2 * Math.pow(x1 / 200, 2) / Math.pow(x3, 3);
    
    return [d_u2_d_x1, d_u2_d_x2, d_u2_d_x3];
}
