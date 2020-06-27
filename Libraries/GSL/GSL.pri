#*********************************************************************
#                                                                    *
# This file is posted by Dr. Stevan Gavrilovic (steva44@hotmail.com) *
# as a result of work performed in the research group of Professor   *
# Terje Haukaas (terje@civil.ubc.ca) at the University of British    *
# Columbia in Vancouver. The code is part of the computer program    *
# Rts, which is an extension of the computer program Rt developed by *
# Dr. Mojtaba Mahsuli (mahsuli@sharif.edu) in the same group.        *
#                                                                    *
# The content of this file is the product of thesis-related work     *
# performed at the University of British Columbia (UBC). Therefore,  *
# the Rights and Ownership are held by UBC.                          *
#                                                                    *
# Please be cautious when using this code. It is provided “as is”    *
# without warranty of any kind, express or implied.                  *
#                                                                    *
# Contributors to this file:                                         *
#   - Stevan Gavrilovic                                              *
#   - Terje Haukaas                                                  *
#                                                                    *
#********************************************************************/

####################################################################
#                                                                  #
#                       GSL PROJECT FILE                           #
#                                                                  #
####################################################################


HEADERS += ./Libraries/GSL/Methods/RLinearSolver/RGSLCholeskyLinearSolver.h \
           ./Libraries/GSL/Methods/RGSLProbabilityDistributions.h \
           ./Libraries/GSL/Methods/RLinearSolver/RGSLHHLinearSolver.h \
           ./Libraries/GSL/Methods/RLinearSolver/RGSLLULinearSolver.h \
           ./Libraries/GSL/Methods/RCholeskyDecomposition/RGSLCholeskyDecomposition.h \
           ./Libraries/GSL/Methods/RDefaultGSLRandomNumberGenerator.h \


SOURCES += ./Libraries/GSL/Methods/RLinearSolver/RGSLCholeskyLinearSolver.cpp \
           ./Libraries/GSL/Methods/RLinearSolver/RGSLHHLinearSolver.cpp \
           ./Libraries/GSL/Methods/RLinearSolver/RGSLLULinearSolver.cpp \
           ./Libraries/GSL/Methods/RCholeskyDecomposition/RGSLCholeskyDecomposition.cpp \
           ./Libraries/GSL/Methods/RDefaultGSLRandomNumberGenerator.cpp \
           ./Libraries/GSL/Methods/RGSLProbabilityDistributions.cpp


INCLUDEPATH +=  ./Libraries/GSL/RDomainAdd \
                ./Libraries/GSL/Methods \
                ./Libraries/GSL/Methods/RLinearSolver \
                ./Libraries/GSL/Methods/RCholeskyDecomposition \
                ./Libraries/GSL/include/gsl \
                ./Libraries/GSL/include \
                ./Libraries/GSL/lib



HEADERS += \
    ./Libraries/GSL/include/gsl/gsl_blas.h \
    ./Libraries/GSL/include/gsl/gsl_blas_types.h \
    ./Libraries/GSL/include/gsl/gsl_block.h \
    ./Libraries/GSL/include/gsl/gsl_block_char.h \
    ./Libraries/GSL/include/gsl/gsl_block_complex_double.h \
    ./Libraries/GSL/include/gsl/gsl_block_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_block_complex_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_block_double.h \
    ./Libraries/GSL/include/gsl/gsl_block_float.h \
    ./Libraries/GSL/include/gsl/gsl_block_int.h \
    ./Libraries/GSL/include/gsl/gsl_block_long.h \
    ./Libraries/GSL/include/gsl/gsl_block_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_block_short.h \
    ./Libraries/GSL/include/gsl/gsl_block_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_block_uint.h \
    ./Libraries/GSL/include/gsl/gsl_block_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_block_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_bspline.h \
    ./Libraries/GSL/include/gsl/gsl_cblas.h \
    ./Libraries/GSL/include/gsl/gsl_cdf.h \
    ./Libraries/GSL/include/gsl/gsl_chebyshev.h \
    ./Libraries/GSL/include/gsl/gsl_check_range.h \
    ./Libraries/GSL/include/gsl/gsl_combination.h \
    ./Libraries/GSL/include/gsl/gsl_complex.h \
    ./Libraries/GSL/include/gsl/gsl_complex_math.h \
    ./Libraries/GSL/include/gsl/gsl_const.h \
    ./Libraries/GSL/include/gsl/gsl_const_cgs.h \
    ./Libraries/GSL/include/gsl/gsl_const_cgsm.h \
    ./Libraries/GSL/include/gsl/gsl_const_mks.h \
    ./Libraries/GSL/include/gsl/gsl_const_mksa.h \
    ./Libraries/GSL/include/gsl/gsl_const_num.h \
    ./Libraries/GSL/include/gsl/gsl_deriv.h \
    ./Libraries/GSL/include/gsl/gsl_dft_complex.h \
    ./Libraries/GSL/include/gsl/gsl_dft_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_dht.h \
    ./Libraries/GSL/include/gsl/gsl_diff.h \
    ./Libraries/GSL/include/gsl/gsl_eigen.h \
    ./Libraries/GSL/include/gsl/gsl_errno.h \
    ./Libraries/GSL/include/gsl/gsl_fft.h \
    ./Libraries/GSL/include/gsl/gsl_fft_complex.h \
    ./Libraries/GSL/include/gsl/gsl_fft_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_fft_halfcomplex.h \
    ./Libraries/GSL/include/gsl/gsl_fft_halfcomplex_float.h \
    ./Libraries/GSL/include/gsl/gsl_fft_real.h \
    ./Libraries/GSL/include/gsl/gsl_fft_real_float.h \
    ./Libraries/GSL/include/gsl/gsl_fit.h \
    ./Libraries/GSL/include/gsl/gsl_heapsort.h \
    ./Libraries/GSL/include/gsl/gsl_histogram.h \
    ./Libraries/GSL/include/gsl/gsl_histogram2d.h \
    ./Libraries/GSL/include/gsl/gsl_ieee_utils.h \
    ./Libraries/GSL/include/gsl/gsl_inline.h \
    ./Libraries/GSL/include/gsl/gsl_integration.h \
    ./Libraries/GSL/include/gsl/gsl_interp.h \
    ./Libraries/GSL/include/gsl/gsl_interp2d.h \
    ./Libraries/GSL/include/gsl/gsl_linalg.h \
    ./Libraries/GSL/include/gsl/gsl_machine.h \
    ./Libraries/GSL/include/gsl/gsl_math.h \
    ./Libraries/GSL/include/gsl/gsl_matrix.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_char.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_complex_double.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_complex_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_double.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_float.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_int.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_long.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_short.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_uint.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_matrix_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_message.h \
    ./Libraries/GSL/include/gsl/gsl_min.h \
    ./Libraries/GSL/include/gsl/gsl_minmax.h \
    ./Libraries/GSL/include/gsl/gsl_mode.h \
    ./Libraries/GSL/include/gsl/gsl_monte.h \
    ./Libraries/GSL/include/gsl/gsl_monte_miser.h \
    ./Libraries/GSL/include/gsl/gsl_monte_plain.h \
    ./Libraries/GSL/include/gsl/gsl_monte_vegas.h \
    ./Libraries/GSL/include/gsl/gsl_multifit.h \
    ./Libraries/GSL/include/gsl/gsl_multifit_nlin.h \
    ./Libraries/GSL/include/gsl/gsl_multilarge.h \
    ./Libraries/GSL/include/gsl/gsl_multimin.h \
    ./Libraries/GSL/include/gsl/gsl_multiroots.h \
    ./Libraries/GSL/include/gsl/gsl_multiset.h \
    ./Libraries/GSL/include/gsl/gsl_nan.h \
    ./Libraries/GSL/include/gsl/gsl_ntuple.h \
    ./Libraries/GSL/include/gsl/gsl_odeiv.h \
    ./Libraries/GSL/include/gsl/gsl_odeiv2.h \
    ./Libraries/GSL/include/gsl/gsl_permutation.h \
    ./Libraries/GSL/include/gsl/gsl_permute.h \
    ./Libraries/GSL/include/gsl/gsl_permute_char.h \
    ./Libraries/GSL/include/gsl/gsl_permute_complex_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_permute_complex_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_float.h \
    ./Libraries/GSL/include/gsl/gsl_permute_int.h \
    ./Libraries/GSL/include/gsl/gsl_permute_long.h \
    ./Libraries/GSL/include/gsl/gsl_permute_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_short.h \
    ./Libraries/GSL/include/gsl/gsl_permute_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_permute_uint.h \
    ./Libraries/GSL/include/gsl/gsl_permute_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_permute_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_char.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_complex_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_complex_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_float.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_int.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_long.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_short.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_uint.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_permute_vector_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_poly.h \
    ./Libraries/GSL/include/gsl/gsl_pow_int.h \
    ./Libraries/GSL/include/gsl/gsl_precision.h \
    ./Libraries/GSL/include/gsl/gsl_qrng.h \
    ./Libraries/GSL/include/gsl/gsl_randist.h \
    ./Libraries/GSL/include/gsl/gsl_rng.h \
    ./Libraries/GSL/include/gsl/gsl_roots.h \
    ./Libraries/GSL/include/gsl/gsl_rstat.h \
    ./Libraries/GSL/include/gsl/gsl_sf.h \
    ./Libraries/GSL/include/gsl/gsl_sf_airy.h \
    ./Libraries/GSL/include/gsl/gsl_sf_bessel.h \
    ./Libraries/GSL/include/gsl/gsl_sf_clausen.h \
    ./Libraries/GSL/include/gsl/gsl_sf_coulomb.h \
    ./Libraries/GSL/include/gsl/gsl_sf_coupling.h \
    ./Libraries/GSL/include/gsl/gsl_sf_dawson.h \
    ./Libraries/GSL/include/gsl/gsl_sf_debye.h \
    ./Libraries/GSL/include/gsl/gsl_sf_dilog.h \
    ./Libraries/GSL/include/gsl/gsl_sf_elementary.h \
    ./Libraries/GSL/include/gsl/gsl_sf_ellint.h \
    ./Libraries/GSL/include/gsl/gsl_sf_elljac.h \
    ./Libraries/GSL/include/gsl/gsl_sf_erf.h \
    ./Libraries/GSL/include/gsl/gsl_sf_exp.h \
    ./Libraries/GSL/include/gsl/gsl_sf_expint.h \
    ./Libraries/GSL/include/gsl/gsl_sf_fermi_dirac.h \
    ./Libraries/GSL/include/gsl/gsl_sf_gamma.h \
    ./Libraries/GSL/include/gsl/gsl_sf_gegenbauer.h \
    ./Libraries/GSL/include/gsl/gsl_sf_hyperg.h \
    ./Libraries/GSL/include/gsl/gsl_sf_laguerre.h \
    ./Libraries/GSL/include/gsl/gsl_sf_lambert.h \
    ./Libraries/GSL/include/gsl/gsl_sf_legendre.h \
    ./Libraries/GSL/include/gsl/gsl_sf_log.h \
    ./Libraries/GSL/include/gsl/gsl_sf_mathieu.h \
    ./Libraries/GSL/include/gsl/gsl_sf_pow_int.h \
    ./Libraries/GSL/include/gsl/gsl_sf_psi.h \
    ./Libraries/GSL/include/gsl/gsl_sf_result.h \
    ./Libraries/GSL/include/gsl/gsl_sf_synchrotron.h \
    ./Libraries/GSL/include/gsl/gsl_sf_transport.h \
    ./Libraries/GSL/include/gsl/gsl_sf_trig.h \
    ./Libraries/GSL/include/gsl/gsl_sf_zeta.h \
    ./Libraries/GSL/include/gsl/gsl_siman.h \
    ./Libraries/GSL/include/gsl/gsl_sort.h \
    ./Libraries/GSL/include/gsl/gsl_sort_char.h \
    ./Libraries/GSL/include/gsl/gsl_sort_double.h \
    ./Libraries/GSL/include/gsl/gsl_sort_float.h \
    ./Libraries/GSL/include/gsl/gsl_sort_int.h \
    ./Libraries/GSL/include/gsl/gsl_sort_long.h \
    ./Libraries/GSL/include/gsl/gsl_sort_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_sort_short.h \
    ./Libraries/GSL/include/gsl/gsl_sort_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_sort_uint.h \
    ./Libraries/GSL/include/gsl/gsl_sort_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_sort_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_char.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_double.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_float.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_int.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_long.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_short.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_uint.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_sort_vector_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_spblas.h \
    ./Libraries/GSL/include/gsl/gsl_specfunc.h \
    ./Libraries/GSL/include/gsl/gsl_splinalg.h \
    ./Libraries/GSL/include/gsl/gsl_spline.h \
    ./Libraries/GSL/include/gsl/gsl_spline2d.h \
    ./Libraries/GSL/include/gsl/gsl_spmatrix.h \
    ./Libraries/GSL/include/gsl/gsl_statistics.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_char.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_double.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_float.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_int.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_long.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_short.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_uint.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_statistics_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_sum.h \
    ./Libraries/GSL/include/gsl/gsl_sys.h \
    ./Libraries/GSL/include/gsl/gsl_test.h \
    ./Libraries/GSL/include/gsl/gsl_types.h \
    ./Libraries/GSL/include/gsl/gsl_vector.h \
    ./Libraries/GSL/include/gsl/gsl_vector_char.h \
    ./Libraries/GSL/include/gsl/gsl_vector_complex.h \
    ./Libraries/GSL/include/gsl/gsl_vector_complex_double.h \
    ./Libraries/GSL/include/gsl/gsl_vector_complex_float.h \
    ./Libraries/GSL/include/gsl/gsl_vector_complex_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_vector_double.h \
    ./Libraries/GSL/include/gsl/gsl_vector_float.h \
    ./Libraries/GSL/include/gsl/gsl_vector_int.h \
    ./Libraries/GSL/include/gsl/gsl_vector_long.h \
    ./Libraries/GSL/include/gsl/gsl_vector_long_double.h \
    ./Libraries/GSL/include/gsl/gsl_vector_short.h \
    ./Libraries/GSL/include/gsl/gsl_vector_uchar.h \
    ./Libraries/GSL/include/gsl/gsl_vector_uint.h \
    ./Libraries/GSL/include/gsl/gsl_vector_ulong.h \
    ./Libraries/GSL/include/gsl/gsl_vector_ushort.h \
    ./Libraries/GSL/include/gsl/gsl_version.h \
    ./Libraries/GSL/include/gsl/gsl_wavelet.h \
    ./Libraries/GSL/include/gsl/gsl_wavelet2d.h


HEADERS += ./Libraries/GSL/RDomainAdd/CreateGSLObjects.h \
           ./Libraries/GSL/RDomainAdd/GSLIncludes.h \
           ./Libraries/GSL/RDomainAdd/GSLMenu.h


win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/Libraries/GSL/lib/ -lgsl -lgslcblas
else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/Libraries/GSL/lib/ -lgsl -lgslcblas
else:macx:LIBS += -L$$_PRO_FILE_PWD_/Libraries/GSL/lib/ -lgsl -lgslcblas
else:unix:LIBS += -L$$_PRO_FILE_PWD_/Libraries/GSL/lib/ -lgsl -lgslcblas







