cmake CMakeLists.txt
make
for num in 1 2 3 4 5 6 7 8 9 10 
do
cat "tests/test${num}.txt" | ./Triangles > ".answer"
if diff .answer answers/answer${num}
then
echo "test ${num} OK"
else
echo "test ${num} Fail"
fi
done
rm ".answer"
for num in 11 12 13 14 15
do
echo "test ${num}"
time cat "tests/test${num}.txt" | ./Triangles > /dev/null
done	
