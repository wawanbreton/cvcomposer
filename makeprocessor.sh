#! /bin/bash

if [[ $@ =~ "--help" ]] || [[ $# < 2 ]]
then
  echo "Usage : makeprocessor.sh PrefixProcessor SubFolder"
  exit 1
fi

lower=${1,,}
upper=${1^^}

folder=processor/${2,,}

if [ ! -d "$folder" ]; then
  mkdir $folder
fi

licence=`cat licence`

############################### HEADER ###############################
echo "$licence

#pragma once

#include \"processor/abstractprocessor.h\"

class ${1}Processor : public AbstractProcessor
{
    public:
        ${1}Processor();

    protected:
        virtual Properties processImpl(const Properties &inputs) override;
};
" > $folder/${lower}processor.h

################################# CPP ################################
echo "$licence

#include \"${lower}processor.h\"

#include \"global/cvutils.h\"


${1}Processor::${1}Processor()
{
}

Properties ${1}Processor::processImpl(const Properties &inputs)
{
    Properties outputs;
    return outputs;
}
" > $folder/${lower}processor.cpp

