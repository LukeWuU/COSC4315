for ((i = 1 ; i <= 10 ; i++))
do
    if (( $i<10));
    then
        echo "Correct python3 in0${i}.py"
        eval $"python ./tcp2/in0${i}.py"
        echo "Our work in0${i}.py"
        eval $"./mini_python ./tcp2/in0${i}.py"
    else
        echo "python3 in${i}.py"
        eval $"python ./tcp2/in${i}.py"
        echo "Our work in${i}.py"
        eval $"./mini_python ./tcp2/in${i}.py"
    fi
done