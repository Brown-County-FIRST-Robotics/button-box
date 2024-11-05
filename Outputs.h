#include <iostream>
#include <vector>
#include "encoding.h"

class Output{
public:
    virtual void Set(bool val)=0;
};


class DigitalJoystickOutput: public Output{
private:
    int index;
public:
    DigitalJoystickOutput(int ind){index=ind;};
    void Set(bool val){
        Joystick.setButton(index, val);
    }
};

class POVCoordinater{
private:
    std::vector<bool> cache={false,false,false};
public:
    void Update(){
        int val=0;
        if(cache[0]){
            val+=45;
        }
        if(cache[1]){
            val+=90;
        }
        if(cache[2]){
            val+=180;
        }
        Joystick.hat(val);
    }
    POVCoordinater(){};
    void UpstreamSet(bool val, int inner_ind){
        cache[inner_ind]=val;
    }
};
class AnalogCoordinater{
private:
    std::vector<bool> cache={false,false,false,false,false,false,false,false};
    int ind;
public:
    void Update(){
        if(ind==5){
            Joystick.sliderRight(encode8ToConstrainedInt(cache))
        }
        if(ind==4){
            Joystick.sliderLeft(encode8ToConstrainedInt(cache))
        }
        if(ind==3){
            Joystick.Zrotate(encode8ToConstrainedInt(cache))
        }
    }
    AnalogCoordinater(int index){ind=index;};
    void UpstreamSet(bool val, int inner_ind){
        cache[inner_ind]=val;
    }
};

class POVOutput:public Output{
private:
    POVCoordinater* coordinater;
    int inner_ind;
public:
    POVOutput(POVCoordinater* c, int ind){
        inner_ind=ind;
        coordinater=c;
    }
    void Set(bool val){
        coordinater->UpstreamSet(val, inner_ind);
    }
};

class AnalogSplitOutput:public Output{
private:
    AnalogCoordinater* coordinater;
    int inner_ind;
public:
    AnalogSplitOutput(AnalogCoordinater* c, int ind){
        inner_ind=ind;
        coordinater=c;
    }
    void Set(bool val){
        coordinater->UpstreamSet(val, inner_ind);
    }
};

class PrintOutput:public Output{
private:
    std::string name;
    bool last_val;
    bool has_set=false;
    void Print(bool v){
        std::cout<< name << " changed to "<<v<<"\n";
    }
public:
    
    PrintOutput(std::string n){
        name=n;
    }
    
    void Set(bool val){
        if(!has_set){
            last_val=!val;
        }
        if(last_val!=val){
            Print(val);
        }
        last_val=val;
    }
};

class MutexCoordinater{
private:
    std::vector<Output*> outputs;
    std::vector<std::pair<bool,bool>> cache;
public:
    MutexCoordinater(std::vector<Output*> outs, int len){
        double poss=std::pow(3,len)+1.0;
        double bits=std::log(poss)/std::log(2.0);
        int bits_needed=std::ceil(bits);
        if(outs.size()!=bits_needed){
            raise std::bad_argument("Mismatch between outputs and length; "+std::to_string(bits_needed)+" bits needed, got "+std::to_string(outs.size()));
        }
        outputs=outs;
        cache={len, {false,false}};
    }
    void Update(){
        PoolMutexEntropy(cache);
        std::vector<bool> ret;
    	for(int i=0; i<outputs.size(); i++) {
    		outputs[i].Set(val&((int) std::pow(2,i)));
    	}
    }
    void UpstreamSet(int inner_ind, bool which, bool val){
        if(which){
            cache[inner_ind].second=val;
        }else{
            cache[inner_ind].first=val;
        }
        Update();
    }
};


class MutexBitOutput: public Output{
private:
    MutexCoordinater* coordinater;
    int index;
    bool which;
public:
    MutexBitOutput(MutexCoordinater* c, int ind, bool w){
        coordinater=c;
        index=ind;
        which=w;
    }
    void Set(bool v){
        coordinater->UpstreamSet(index,which,v);
    }
}



POVCoordinater povCoord{};
AnalogCoordinater analogCoord0{3};
AnalogCoordinater analogCoord1{4};
AnalogCoordinater analogCoord2{5};

int digital_outputs=21;
int pov_bits=3;
int analog_bits=8;



// This is the magic function that just allocates a bit
// It can allocate indices 1-48 (inclusive)
// It allocates like this (all inclusive):
// 1-21: direct to digital buttons
// 22-24: encodes it to POV
// 25-32: encodes it into analog axis 3
// 33-40: encodes it into analog axis 4
// 41-48: encodes it into analog axis 5
Output* alloc(int ind){
    int curr_max=digital_outputs;
    if(ind<=curr_max){
        return new DigitalJoystickOutput(ind);
    }
    curr_max+=pov_bits;
    if(ind<=curr_max){
        return new POVOutput(&povCoord, ind-curr_max+pov_bits-1);
    }
    curr_max+=analog_bits;
    if(ind<=curr_max){
        return new AnalogSplitOutput(&analogCoord0,ind-curr_max+analog_bits-1);
    }
    curr_max+=analog_bits;
    if(ind<=curr_max){
        return new AnalogSplitOutput(&analogCoord1,ind-curr_max+analog_bits-1);
    }
    curr_max+=analog_bits;
    if(ind<=curr_max){
        return new AnalogSplitOutput(&analogCoord2,ind-curr_max+analog_bits-1);
    }
    return new PrintOutput("(Overflow) Joystick index "+std::to_string(ind));
}
