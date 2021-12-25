
int intValue;
double doubleValue;

int spinUpCpu()
{
    for( int i = 0; i < 1000000; ++i )
    {
        intValue += i * i;
    }
    return intValue;
}

const char * smallIntegers[] = {  "123", "18", "223", "1", "19", "71", "73", "34", "12", "43", "60", 
                                  "10", "318", "22", "1", "189", "791", "7223", "334", "121", "43", "60", nullptr };

const char * largeIntegers[] =
{
    "3123711",
    "61377767",
    "284852",
    "5626661",
    "64264561",
    "76787812",
    "9725680",
    "33724764",
    "1154454",
    "45673323",
    "67657540",
    "3577702",
    "11563813",
    "256737",
    "1874",
    "17756726",
    "7155646",
    "6576753",
    "3572124",
    "1767652",
    "42555231",
    "64564660",
    nullptr
};

const char * hugeIntegers[] =
{
    "31233423423711",
    "61377762322357",
    "284823552343452",
    "56262332e323661",
    "6426423535561",
    "76781212123127812",
    "972512321213680",
    "33724765462364",
    "115446754545754",
    "45673666363323",
    "67657767556540",
    "357752545425702",
    "115634242342813",
    "25674242342337",
    "1842343242374",
    "17756423423423726",
    "715562434342346",
    "6576423424753",
    "35724234234124",
    "17676423423452",
    "42555242342342331",
    "645646424243260",
    nullptr
};
