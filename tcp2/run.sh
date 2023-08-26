for ((i = 1 ; i <= 15 ; i++))
do
    if (( $i<10));
    then
        echo "python in0${i}.py"
        eval $"python in0${i}.py"
    else
        echo "python in${i}.py"
        eval $"python in${i}.py"
    fi
done
