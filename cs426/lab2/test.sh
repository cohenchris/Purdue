#!/bin/bash

ciphertxt=("e2f609b0002fd83715d30f1aab696ea22364b67a97db1f4e96f14c83b48e6195" "e2f609b0002fd83715d30f1aab696ea22ef02f5e0384b3c7f99fdb9779409a14" "7b9346903162497c23f3835d6488f361a5787e53cde5e89043e1946b07c8ee7aab7719aedbb4ba94fc9cc9ca936d91b8")
plaintxt=("6461766574690a0a0a0a0a0a0a0a0a0a" "68616e64736f6d650808080808080808" "6e642d692d73747564792d63732d61742d7075726475652d7765656565650202")
for i in ${!ciphertxt[@]};
do
        cipher=${ciphertxt[$i]}
        plain=${plaintxt[$i]}

        result=`./attack.py -a $cipher`
        if [ "$plain" !=  "$result" ]; then
                echo "Failure."
        else
                echo "Success."
        fi
done