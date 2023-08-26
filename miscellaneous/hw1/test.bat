ECHO OFF
ECHO ----------

::fc tc01.out myans1.txt

for /L %%I in (1,1,10) do (
    if %%I LSS 10 (
        python3 freqnumber.py "k=3;input=tc0%%I.txt;output=myans%%I.txt"
    ) else (
        python3 freqnumber.py "k=3;input=tc%%I.txt;output=myans%%I.txt"
    )
)

for /L %%I in (1,1,9) do fc "tc0%%I.out" "myans%%I.txt"
fc tc10.out myans10.txt