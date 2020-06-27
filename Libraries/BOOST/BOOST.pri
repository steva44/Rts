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
#                                                                    *
#********************************************************************/

####################################################################
#                                                                  #
#                    RTS BOOST PROJECT FILE                        #
#                                                                  #
####################################################################


 INCLUDEPATH +=  . \
                    ./Libraries/BOOST/include \
                    ./Libraries/BOOST/include/boost \
                    ./Libraries/BOOST/include/boost/unordered \
                    ./Libraries/BOOST/include/boost/date_time \
                    ./Libraries/BOOST/include/boost/bind \
                    ./Libraries/BOOST/include/boost/config \
                    ./Libraries/BOOST/include/boost/integer \
                    ./Libraries/BOOST/include/boost/container \
                    ./Libraries/BOOST/include/boost/iterator \
                    ./Libraries/BOOST/include/boost/mpl \
                    ./Libraries/BOOST/include/boost/math \
                    ./Libraries/BOOST/include/boost/functional \
                    ./Libraries/BOOST/include/boost/detail \
                    ./Libraries/BOOST/include/boost/type_traits \
                    ./Libraries/BOOST/include/boost/range \
                    ./Libraries/BOOST/include/boost/fusion \
                    ./Libraries/BOOST/include/boost/lexical_cast \
                    ./Libraries/BOOST/include/boost/optional \
                    ./Libraries/BOOST/include/boost/numeric \
                    ./Libraries/BOOST/include/boost/preprocessor \
                    ./Libraries/BOOST/include/boost/utility \
                    ./Libraries/BOOST/include/boost/move \
                    ./Libraries/BOOST/include/boost/core \
                    ./Libraries/BOOST/include/boost/concept \
                    ./Libraries/BOOST/include/boost/tuple \
                    ./Libraries/BOOST/include/boost/exception \
                    ./Libraries/BOOST/include/boost/function \
                    ./Libraries/BOOST/include/boost/uuid \
                    ./Libraries/BOOST/include/boost/random \
                    ./Libraries/BOOST/include/boost/pending \
                    ./Libraries/BOOST/include/boost/smart_ptr \
                    ./Libraries/BOOST/include/boost/predef \
                    ./Libraries/BOOST/include/boost/io \
                    ./Libraries/BOOST/include/boost/algorithm



#win32:CONFIG(release, debug|release): LIBS += -L$$_PRO_FILE_PWD_/Libraries/BOOST/lib/release/ -lboost_atomic -lboost_math_tr1f -lboost_chrono -lboost_math_tr1l -lboost_container -lboost_context -lboost_prg_exec_monitor -lboost_coroutine -lboost_program_options -lboost_date_time  -lboost_fiber -lboost_random -lboost_filesystem -lboost_regex -lboost_graph -lboost_serialization -lboost_iostreams -lboost_signals -lboost_locale -lboost_system -lboost_log -lboost_thread -lboost_log_setup -lboost_timer -lboost_math_c99 -lboost_type_erasure -lboost_math_c99f -lboost_unit_test_framework -lboost_math_c99l -lboost_wave -lboost_math_tr1 -lboost_wserialization
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$_PRO_FILE_PWD_/Libraries/BOOST/lib/debug/ -lboost_atomic -lboost_math_tr1f -lboost_chrono -lboost_math_tr1l -lboost_container -lboost_context -lboost_prg_exec_monitor -lboost_coroutine -lboost_program_options -lboost_date_time  -lboost_fiber -lboost_random -lboost_filesystem -lboost_regex -lboost_graph -lboost_serialization -lboost_iostreams -lboost_signals -lboost_locale -lboost_system -lboost_log -lboost_thread -lboost_log_setup -lboost_timer -lboost_math_c99 -lboost_type_erasure -lboost_math_c99f -lboost_unit_test_framework -lboost_math_c99l -lboost_wave -lboost_math_tr1 -lboost_wserialization
#else:macx: LIBS += -L$$_PRO_FILE_PWD_/Libraries/BOOST/lib/ -lboost_atomic -lboost_math_tr1f -lboost_chrono -lboost_math_tr1l -lboost_container -lboost_context -lboost_prg_exec_monitor -lboost_coroutine -lboost_program_options -lboost_date_time -lboost_fiber -lboost_random -lboost_filesystem -lboost_regex -lboost_graph -lboost_serialization -lboost_iostreams -lboost_signals -lboost_locale -lboost_system -lboost_log -lboost_thread -lboost_log_setup -lboost_timer -lboost_math_c99 -lboost_type_erasure -lboost_math_c99f -lboost_unit_test_framework -lboost_math_c99l -lboost_wave -lboost_math_tr1 -lboost_wserialization

#INCLUDEPATH += $$_PRO_FILE_PWD_/Libraries/BOOST/include
#DEPENDPATH += $$_PRO_FILE_PWD_/Libraries/BOOST/include
