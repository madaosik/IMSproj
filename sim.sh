#!/bin/bash

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -n)
    itercnt="$2"
    shift # past argument
    shift # past value
    ;;
    -f)
    fermions="$2"
    shift # past argument
    shift # past value
    ;;
    -m)
    matrixsize="$2"
    shift # past argument
    shift # past value
    ;;
    -a)
    alpha="$2"
    shift
    shift
    ;;
    -s)
    js="$2"
    shift
    shift
    ;;
    -d)
    jd="$2"
    shift
    shift
    ;;
    -o)
    jo="$2"
    shift
    shift
    ;;
    -b)
    beta="$2"
    shift
    shift
    ;;
    -sens_sim)
    sens_sim="$2"
    steps="$3"
    incr_factor="$4"
    shift
    shift
    shift
    shift
    ;;
esac
done


if [ -z ${sens_sim+x} ]; then
	steps=1
else
	echo ""
	echo -e "** ANALYZA CITLIVOSTI NA PARAMETR $sens_sim, NARUSTAJICIM/KLESAJICIM CELKEM $steps x O FAKTOR $incr_factor PRO KAZDOU SADU SIMULACI **"
	echo ""
fi

if [ -z ${itercnt+x} ]; then itercnt=5; fi

echo "Spoustim sadu $itercnt simulaci s parametry:"

echo -e "\t** rozmery mistnosti:\t\t"$matrixsize"x"$matrixsize""
echo -e "\t** pocet pritomnych fermionu:\t"$fermions""
echo -e "\t** Parametry sady simulaci:"

if [ -z ${alpha+x} ]; then 
	alpha=0.5; 
	echo -e "\t\talfa (pravd. odstraneni d-bosonu po neobsazeni):\t\t\t$alpha (implicitne)"; 
else 
	echo -e "\t\talfa (pravd. odstraneni d-bosonu po neobsazeni):\t\t\t"$alpha""; 
fi

if [ -z ${js+x} ]; then 
	js=2; 
	echo -e "\t\tJ-s (vazba fermionu na staticke pozemni pole):\t\t\t\t$js (implicitne)"; 
else 
	echo -e "\t\tJ-s (vazba fermionu na staticke pozemni pole):\t\t\t\t"$js""; 
fi

if [ -z ${jd+x} ]; then 
	jd=1; 
	echo -e "\t\tJ-d (vazba fermionu na dynamicke pozemni pole):\t\t\t\t$jd (implicitne)"; 
else 
	echo -e "\t\tJ-d (vazba fermionu na dynamicke pozemni pole):\t\t\t\t"$jd""; 
fi

if [ -z ${jo+x} ]; then 
	jo=1; 
	echo -e "\t\tJ-o (koeficient setrvacnosti smeru pohybu fermionu):\t\t\t$jo (implicitne)"; 
else 
	echo -e "\t\tJ-o (koeficient setrvacnosti smeru pohybu fermionu):\t\t\t"$jo""; 
fi

if [ -z ${beta+x} ]; then 
	beta=10; 
	echo -e "\t\tbeta (eliminacni konst. ovlivn. silu setrvacnosti a odpor jit zpet):\t$beta (implicitne)"; 
else 
	echo -e "beta (eliminacni konst. ovlivn. silu setrvacnosti a odpor jit zpet):\t"$beta""; 
fi

echo -e "\n"
echo -e "Vysledne hodnoty:"

for sens_run in $(seq 1 $steps); do
	total=0
	for run in $(seq 1 $itercnt); do
			result=$(./IMS_CA -f $fermions -m $matrixsize -a $alpha -b $beta -s $js -d $jd -o $jo | cut -d':' -f2-)
			echo -e "\tpocet iteraci v behu $run: $result"
			total=$((total+result))
	
	done
	if ! [[ -z ${sens_sim+x} ]]; then
		case $sens_sim in
			a)
			alpha=$(echo "scale=2;$alpha*$incr_factor" | bc -l)
			newval=$alpha
			;;
			b)
			beta=$(echo "scale=2;$beta*$incr_factor" | bc -l)
			newval=$beta
			;;
			m)
			matrixsize=$((matrixsize*incr_factor))
			newval=$matrixsize
			;;
			f)
			fermions=$((fermions*incr_factor))
			newval=$fermions
			;;
			s)
			js=$(echo "scale=2;$js*$incr_factor" | bc -l)
			newval=$js
			;;
			d)
			jd=$(echo "scale=2;$jd*$incr_factor" | bc -l)
			newval=$jd
			;;
			o)
			jo=$(echo "scale=2;$jo*$incr_factor" | bc -l)
			newval=$jo
			;;
		esac
		total=$(( total/itercnt ))
		echo ""
		echo -e "Beh cislo $sens_run analyzy citlivosti na parametr '$sens_sim' dokoncen." 
		echo -e "** Prumerny pocet iteraci: $total"
		echo -e "-----------------------------------------\n"
		echo -e "Nova hodnota '$sens_sim': $newval"
	fi
done	

if [ -z ${sens_sim+x} ]; then
	total=$((total/itercnt))
	echo -e "\t** Prumerny dosazeny pocet iteraci v ramci simulacni sady: $total"
fi
