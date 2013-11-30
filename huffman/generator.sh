unset data
for i in {0..10000}
do
	r=$(expr $RANDOM % 3)
	if [ $r -eq 0 ]
	then
		data=a$data
	elif [ $r -eq 1 ]
	then
		data=b$data
	else
		data=c$data
	fi
done

echo $data