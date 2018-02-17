rm bin/*
rm test_results.txt

gcc test_cases/IF_T.c test_cases/IF_T_driver.c -o bin/IF_T
bin/IF_T
echo IF_T >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/IF_F.c test_cases/IF_F_driver.c -o bin/IF_F
bin/IF_F
echo IF_F >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/IF_ELSE_T.c test_cases/IF_ELSE_T_driver.c -o bin/IF_ELSE_T
bin/IF_ELSE_T
echo IF_ELSE_T >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/IF_ELSE_F.c test_cases/IF_ELSE_F_driver.c -o bin/IF_ELSE_F
bin/IF_ELSE_F
echo IF_ELSE_F >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/FOR_N.c test_cases/FOR_N_driver.c -o bin/FOR_N
bin/FOR_N
echo FOR_N >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/LOCAL_ARRAY.c test_cases/LOCAL_ARRAY_driver.c -o bin/LOCAL_ARRAY
bin/LOCAL_ARRAY
echo LOCAL_ARRAY >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/SELF_RECURSION.c test_cases/SELF_RECURSION_driver.c -o bin/SELF_RECURSION
bin/SELF_RECURSION
echo SELF_RECURSION >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/GLOBAL.c test_cases/GLOBAL_driver.c -o bin/GLOBAL
bin/GLOBAL
echo GLOBAL >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/MAIN.c test_cases/MAIN_driver.c -o bin/MAIN
bin/MAIN
echo MAIN >> test_results.txt
echo $? >> test_results.txt

gcc test_cases/ATOF.c test_cases/ATOF_driver.c -o bin/ATOF
bin/ATOF
echo ATOF >> test_results.txt
echo $? >> test_results.txt

