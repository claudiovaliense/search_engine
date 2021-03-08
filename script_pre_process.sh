pre_process(){
    for index_doc in $(seq 0 1000068); do
        sed -i -e 's/\(.*\)/\L\1/;s/[ãāáǎàâ]/a/g;s/[êēéěè]/e/g;s/[īíǐ]/i/g;s/[õōóǒôò]/o/g;s/[ūúǔùǖǘǚǜ]/u/g;s/[ç]/c/g;s/[,]/ , /g;s/[.]/ . /g;s/[!]/ ! /g;s/[+]/ + /g;s/\x27/ \x27 /g;s/\x22/ \x22 /g;s/(/ ( /g;s/)/ ) /g;s/\[/ [ /g;s/]/ ] /g;s/?/ ? /g;s/#/ # /g' dataset_tp/${index_doc}.txt
    done
}

pre_process
