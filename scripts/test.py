import math;

def genSin(num_samples):
    s = "{"
    for i in range(0,num_samples):
        fval = math.sin(2*math.pi*i/num_samples);
        fval += 1;
        fval /= 2;
        fval*=4095;
        val = format((int)(fval),"03x")
        s += "0x"+val + ","
        #s+=""+str(int(fval))+",";
    s = s[:-1]+"}"
    print(s)

def genSquare(num_samples):
    s = "{"
    for i in range(0,num_samples):
        fval = 0xfff;
        if(i < num_samples/2):
            fval = 0
        val = format((int)(fval),"03x")
        s += "0x"+val + ","
        #s+=""+str(int(fval))+",";
    s = s[:-1]+"}"
    print(s)

def genSaw(num_samples):
    s = "{"
    for i in range(0,num_samples):
        fval = 0xfff*(1-(i/(num_samples-1)));
        val = format((int)(fval),"03x")
        s += "0x"+val + ","
    s = s[:-1]+"}"
    print(s)


genSaw(512);
