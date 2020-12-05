#!/bin/bash

test1_score=0
test2_score=0
test3_score=0
test4_score=0
test5_score=0
test6_score=0
test7_score=0
test8_score=0
test9_score=0
test10_score=0
test11_score=0


final_score=0


# test1: invalid length

echo "" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "abcd5" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "Error: password length invalid\nError: password length invalid\nError: password length invalid")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test1_score=4
fi


# test2: check if points are correctly given for length
# test3: check for alphanumeric

echo "aBcDeF" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "aBcDeFgHiJkLmNo" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "aBcDeFgHiJkLmNoPqRsTuVwXyZaBcDe" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "11\n20\n36")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test2_score=4
	test3_score=4
fi


# test4: check for numbers

echo "aBc9F1" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "aBcDeFgHiJk0mN2" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "12a34b56c78d" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "aBcDeFg94JkLmNo8qRsTuVwXyZaBcD2" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "16\n25\n22\n41")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test4_score=4
fi



# test5: check for special chars

echo "aBcDeF#" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "aBcDeF$" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "aBcDeF+" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "aBcDeF%" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "aBcDeF@" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "aBcDeF\\" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "17\n17\n17\n17\n17\n12")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test5_score=4
fi


# test6: check for repeated alphanumeric characters

echo "aBcDeFgg" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "aBc55eF" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "AbCdEfGG" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "3\n7\n3")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test6_score=4
fi



# test7: check for 3 consecutive lowercase

echo "abcDeFg" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "aBcDeFgHiJkawfa" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "abcDefgHijkL" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "abcdefghijkl" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "9\n17\n14\n14")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test7_score=4
fi


# test8: check for 3 consecutive uppercase

echo "ABCdEfG" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "AbCdEfGhIjKAWFA" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "ABCdEFGhIJKl" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "ABCDEFGHIJKL" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_resul=$(cat _temp_results.txt)
expected_result=$(echo -e "9\n17\n14\n14")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test8_score=4
fi


# test9: check for 3 consecutive numbers

echo "123456" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "12a345b67A" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "ab12345f2456gD986f2e35a6f" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "8\n17\n32")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test9_score=4
fi



# test10: test everything together

echo "ThisBlowsMindsss189" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" > _temp_results.txt

echo "abcDEF123" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "Exon#Mobi\\Le21" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "ILIveinStreet#1" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

echo "123456789abcdef@gDWSS@Aw4" > _temp_input.txt
./pwcheck.sh "_temp_input.txt" >> _temp_results.txt

program_result=$(cat _temp_results.txt)
expected_result=$(echo -e "13\n10\n26\n14\n21")

if [[ "${program_result}" == "${expected_result}" ]]; then
	test10_score=10
fi



#if program did not crash, add points for robustness
test11_score=4


#calculate final score
(( final_score = test1_score + test2_score + test3_score + test4_score + test5_score + test6_score + test7_score + test8_score + test9_score + test10_score + test11_score ))


#display results
printf "\n\n"
echo   "*----------------------------------------------------------------------------------*"
echo -e "|   Password Scorer Grading Results:                                               |" 
echo   "|----------------------------------------------------------------------------------|"
echo   "|   Error checking                                              =  ${test1_score} out of 4      |"
echo   "|   Password length                                             =  ${test2_score} out of 4      |"
echo   "|   At least one aplhabet [A-Z,a-z]                             =  ${test3_score} out of 4      |"
echo   "|   At least one number [0-9]                                   =  ${test4_score} out of 4      |"
echo   "|   At least one special char (#$+%@)                           =  ${test5_score} out of 4      |"
echo   "|   Repetition of the same alphanumeric character (i.e. aa,bbb) =  ${test6_score} out of 4      |"
echo   "|   3 consecutive lowercase                                     =  ${test7_score} out of 4      |"
echo   "|   3 consecutive uppercase                                     =  ${test8_score} out of 4      |"
echo   "|   3 consecutive numbers                                       =  ${test9_score} out of 4      |"
printf "|   Multiple regular expressions working together correctly     =  %-2s out of 10    |\n" "${test10_score}"
echo   "|   Robustness (did not crash while testing)                    =  ${test11_score} out of 4      |"
echo   "|----------------------------------------------------------------------------------|"
printf "|   Total:                                                      =  %2s out of 50    |\n" "${final_score}"
echo   "*----------------------------------------------------------------------------------*"
printf "\n\n"

#echo ${temp}

rm _temp_input.txt
rm _temp_results.txt
