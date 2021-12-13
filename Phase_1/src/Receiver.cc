//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Receiver.h"
using namespace std;

Define_Module(Receiver);

void Receiver::initialize()
{
    // TODO - Generated method body
}

string xor1(string a, string b)
{

    // Initialize result
    string result = "";

    int n = b.length();

    // Traverse all bits, if bits are
    // same, then XOR is 0, else 1
    for(int i = 1; i < n; i++)
    {
        if (a[i] == b[i])
            result += "0";
        else
            result += "1";
    }
    return result;
}

// Performs Modulo-2 division
string mod2div(string divident, string divisor)
{

    // Number of bits to be XORed at a time.
    int pick = divisor.length();

    // Slicing the divident to appropriate
    // length for particular step
    string tmp = divident.substr(0, pick);

    int n = divident.length();

    while (pick < n)
    {
        if (tmp[0] == '1')

            // Replace the divident by the result
            // of XOR and pull 1 bit down
            tmp = xor1(divisor, tmp) + divident[pick];
        else

            // If leftmost bit is '0'.
            // If the leftmost bit of the dividend (or the
            // part used in each step) is 0, the step cannot
            // use the regular divisor; we need to use an
            // all-0s divisor.
            tmp = xor1(std::string(pick, '0'), tmp) +
                  divident[pick];

        // Increment pick to move further
        pick += 1;
    }

    // For the last n bits, we have to carry it out
    // normally as increased value of pick will cause
    // Index Out of Bounds.
    if (tmp[0] == '1')
        tmp = xor1(divisor, tmp);
    else
        tmp = xor1(std::string(pick, '0'), tmp);

    return tmp;
}

// Function used at the sender side to encode
// data by appending remainder of modular division
// at the end of data.
string GenerateCRC(string data, string key)   //data and key are strings in binary form   [For Hamming Distance of 2 use this key: 11100111]
{
    int l_key = key.length();

    // Appends n-1 zeroes at end of data
    string appended_data = (data +std::string(l_key - 1, '0'));
    string remainder = mod2div(appended_data, key);
    return remainder;

    // Append remainder in the original data
    //string codeword = data + remainder;
    cout << "Remainder : " << remainder << "\n";
    //cout << "Encoded Data (Data + Remainder) :" << codeword << "\n";
}

vector<bitset<8> > FrameMessage(vector<bitset<8> > Msg)
{
    //string Flag="$";   //Equivalent to 00100100
    bitset<8> Flag=00100100;
    //string Escape="/";   //Equivalent to 00101111
    bitset<8> Escape=00101111;
    vector<bitset<8> > Payload;
    Payload.push_back(Flag);    //Insert starting Flag
    for(int i=0; i<Msg.size(); i++)
    {
        if (Msg[i]==00101111)
        {
            Payload.push_back(00101111);
            Payload.push_back(00101111);
            Payload.push_back(00101111);
        }
        else
        {
            Payload.push_back(Msg[i]);
        }
    }
    Payload.push_back(Flag);    //Insert Ending Flag
}

void Receiver::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    MyMessage_Base *mmsg=check_and_cast<MyMessage_Base*>(msg);
    std::cout << "Receiver message Sequence number is " << mmsg->getSeq_Num() << endl;
    std::cout << "Receiver Payload is " << mmsg->getM_Payload() << endl;
    std::cout << "Receiver Check bits are " << mmsg->getMycheckbits().to_string() << endl;
    std::cout << "Message ID: " << mmsg->getSeq_Num() << endl;

    //std::cout<<endl<<"Received message: "<<msg->getName()<<endl;
    //Received Message
    std::string ReceivedMessage = mmsg->getM_Payload();
    int Header=ReceivedMessage.length() +2;
    std::bitset<8> HeaderBits(Header);

    //Convert to bitsets
    std::vector<std::bitset<8> >vec;   //Contains Message Only
    for(int i = 0 ; i < ReceivedMessage.size(); i++)
    {
      std::bitset<8> input((int)ReceivedMessage[i]);
      vec.push_back(input);
     }

    //old parity
    std::bitset<8> ReceivedParity=mmsg->getMycheckbits();

    //Calculate New Parity
    std::bitset<8> NewParity(HeaderBits);
    for(int i = 0 ; i < ReceivedMessage.size(); i++)
     {
         std::bitset<8> input((int)ReceivedMessage[i]);
         NewParity = NewParity ^ input;
     }

    //std::cout<<ReceivedParity<<endl<<NewParity<<endl;

    //Checking parity
    MyMessage_Base *Result=check_and_cast<MyMessage_Base*>(msg);
    if(ReceivedParity == NewParity)
    {
        Result->setM_Type(1);   //Positive Acknowledge if data is right
        //std::cout<<"The Original Message is: ";
        //for(int i = 1 ; i < ReceivedMessage.size()-1; i++)
        //{
            //std::cout<<(char)vec[i].to_ulong();
        //}
        //std::cout<<endl;
    }

    else
    {
        Result->setM_Type(2);
        //std::cout<<"Error Message"<<endl;

    }

    send(Result,"out");
    exit(0);

}
