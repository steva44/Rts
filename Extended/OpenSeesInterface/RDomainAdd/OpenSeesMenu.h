

//if (className == QString("MODELS").remove(' ')) {
//    result << "Structural Analysis Models";
//}

//if (className == QString("METHODS").remove(' ')) {
//    result << "Structural Analyses"
//           << "Structural Assemblers";
//}


//if (className == QString("METHODS Structural Analyses").remove(' ')) {
//    result << "Linear Static"
//           << "Linear Dynamic"
//           << "Inelastic Static"
//           << "Inelastic Dynamic";
//}

//if (className == QString("METHODS Structural Assemblers").remove(' ')) {
//    result << "Transformation Matrix"
//           << "Connectivity Table";
//}



//if (className == QString("OTHER OBJECTS").remove(' ')) {
//    result<< "Nodes"
//          << "Parametric Points"
//          << "Slave Nodes";
//}


////Add the Elements item in the menu...
//if (className == QString("Objects").remove(' ')) {
//    result << "ELEMENTS";
//}


//if (className == QString("ELEMENTS").remove(' ')) {
//    result << "Elastic Frame Element"
//           << "Elastic Truss Element"
//           << "Elastic Plate Element";
//}


//Add the materials item in the menu...
if (className == QString("Objects").remove(' ')) {
    result << "MATERIALS";
}


if (className == QString("MATERIALS").remove(' ')) {
    result<< "OpenSees Concrete01"
          << "OpenSees Concrete02"
          << "OpenSees Concrete04"
          << "OpenSees Steel02"
          << "OpenSees ElasticBilin";
}



