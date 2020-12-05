#!/bin/bash
#Grading script for Lab 2 CS252
GRADE=0

#check if error for too short of password works +2
echo "short" > test
./password.sh test > out
echo "Error: Password length invalid." > errortest

if diff errortest out; then
	(( GRADE = GRADE + 2 ))
fi

#check if error for too long of password works +3
echo "waytoolongofapasswordsothisreallyshouldfailforsure" > test
./password.sh test > out
if diff errortest out; then
	(( GRADE = GRADE + 3 ))
fi

#check if character count and alpha and number characters work +15 
echo "he1lo2" > test
./password.sh test > out
echo "Score: 16" > expected
if diff expected out; then
	(( GRADE = GRADE + 15 ))
fi

#check if special characters work
echo "h%a@o#" > test
./password.sh test > out
echo "Score: 16" > expected
if diff expected out; then
	(( GRADE = GRADE + 5 ))
fi

#check if repetition of characters is detected
echo "h1e2ll3" > test
./password.sh test > out
echo "Score: 7" > expected
if diff expected out; then
	(( GRADE = GRADE + 5 ))
fi

#check if consecutive lowercase alpha characters is detected
echo "hel20w" > test
./password.sh test > out
echo "Score: 13" > expected
if diff expected out; then
	(( GRADE = GRADE + 5 ))
fi

#check if consecutive uppercase alpha characters is detected
echo "HEL20wX" > test
./password.sh test > out
echo "Score: 14" > expected
if diff expected out; then
	(( GRADE = GRADE + 5 ))
fi

#check if consecutive numbers are detected
echo "2013isHEre" > test
./password.sh test > out
echo "Score: 22" > expected
if diff expected out; then
	(( GRADE = GRADE + 5 ))
fi

#check for multiple expressions working together
echo "#2013isthebeestYearEverz" > test
./password.sh test > out
echo "Score: 28" > expected
if diff expected out; then
	(( GRADE = GRADE + 10 ))
fi

#script did not crash
(( GRADE = GRADE + 5 ))

#print score and cleanup
echo Add up to 5 points based on how clean their script looks
echo Total Score: ${GRADE} / 60
rm test
rm out
rm expected
rm errortest
