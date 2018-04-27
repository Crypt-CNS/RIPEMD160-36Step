#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
using namespace std;

#define LENGTH 32
#define WORD_NUM 16
#define VALIDNUM 0xfcf2100
const long long M = 1024;
const long long MOD = M*M*M * 4;

/*rotate left*/
unsigned int LL(unsigned int number, int r){
	number = (number >> (32 - r)) | (number << r);
	return number;
}

/*rotate right*/
unsigned int RR(unsigned int number, int r){
	number = (number << (32 - r)) | (number >> r);
	return number;
}

/*Get the certain bit*/
inline unsigned int getBit(unsigned int number,int position){
	return (number >> position) & 0x1;
}

/*round function XOR*/
unsigned int XOR(unsigned int x, unsigned int y, unsigned int z){
	return x^y^z;
}

/*round function IFX*/
unsigned int IFX(unsigned int x, unsigned int y, unsigned int z){
	return (x&y) ^ ((~x)&z);
}

/*round function XOR*/
unsigned int ONZ(unsigned int x, unsigned int y, unsigned int z){
	return (x | (~y)) ^ z;
}

/*round function XOR*/
unsigned int IFZ(unsigned int x, unsigned int y, unsigned int z){
	return (x&z) ^ (y&(~z));
}

/*round function XOR*/
unsigned int ONX(unsigned int x, unsigned int y, unsigned int z){
	return x ^ (y | (~z));
}

//Due to the limitation of my PC, we can't give the implementation to
//solve such an equation as explained in Section 4. However, once the 
//computer is good enough, it can be implemented easily. Here, we use
//a similar way introduced in [12] to solve it and the time complexity 
//is 2^7.
/*solve the equation: c<<<15 + t = (c+r)<<<9 , r and t is known*/
void solveEqation(unsigned int r,unsigned int t, int &length,unsigned int value[]){
	 //Guess C17,18,19,20,21,22,23
	//m=c<<<15 + t
	//s=c+r
	length = 0;
	unsigned int m = 0,mt=0;
	unsigned int s = 0,st=0;
	unsigned int rt = 0;
	unsigned int carry = 0;
	unsigned int c = 0,ct=0;
	for (unsigned i = 0; i < 0x80; i++){//i 表示 C17-23位的值
		c = (i << 17) & 0x00fe0000;
		carry = 0;

		//compute M{6-0}
		m = (i + t)&0x7f;//Only M[6-0] is valid
		//compute S[23-29]
		mt = m;
		st = m;
		s = (m << 23)&0x3f800000;
		//check the carry to the 24th bit from the 23rd bit.

		if (getBit(s, 23) == 0 && (getBit(c, 23) | getBit(r, 23) == 1)){
			carry = 1;
		}
		else if (getBit(s, 23) == 1 && (getBit(c, 23) ==1 && getBit(r, 23) == 1)){
			carry = 1;
		}
		/*else if ((s & 0x00800000) == 0x00800000 && (c & 0x00800000) == 0x00800000 && (r & 0x00800000) == 0x00800000){
			carry = 1;
		}*/
		//计算C[24-29]
		rt = (r << 2) >> 26;
		st = st >> 1;
		ct = st - rt - carry;
		c = ((ct << 24) & 0x3f000000)|c;

		//compute M[12-7]
		carry = 0;
		m = LL(c, 15);
		m = m + t;
		m = (m >> 7) & 0x3f;
		//compute S[30,31,0,1,2,3]
		st = m;
		s = s | RR(st, 2);
		//compute C[30,31,0,1,2,3]
		//compute C[0,1,2,3]
		ct = (st >> 2) - (r & 0xf);
		c = c | (ct & 0xf);
		//compute C[30,31]
		//check the carry to the 30th bit from the 29th bit
		if (getBit(s, 29) == 0 && (getBit(c, 29) | getBit(r, 29)) == 1){
			carry = 1;
		}
		/*if (s & 0x20000000 == 0 && ((c & 0x20000000) | (r & 0x20000000) == 0x20000000)){
			carry == 1;
		}*/
		else if (getBit(s, 29) == 1 && getBit(c, 29) == 1 && getBit(r, 29) == 1){
			carry = 1;
		}
		/*else if ((s & 0x20000000) == 0x20000000 && (c & 0x20000000) == 0x20000000 && (r & 0x20000000) == 0x20000000){
			carry = 1;
		}*/
		ct = (st & 0x3) - carry - (r >> 30);
		c = c | ((ct << 30) & 0xc0000000);

		//compute M[18-13]
		carry = 0;
		m = LL(c, 15);
		m = m + t;
		m = (m >> 13) & 0x3f;
		//compute S[9-4]
		st = m;
		s = s | (st << 4);
		//compute C[9-4]
		//check the carry to the 4th bit from the 3rd bit.
		if (getBit(s, 3) == 0 && (getBit(c, 3) | getBit(r, 3)) == 1){
			carry = 1;
		}
		/*if (s & 0x00000008 == 0 && ((c & 0x00000008) | (r & 0x00000008) == 0x8)){
			carry == 1;
		}*/
		else if (getBit(s, 3) == 1 && getBit(c, 3) == 1 && getBit(r, 3) == 1){
			carry = 1;
		}
		/*else if ((s & 0x8) == 0x8 && (c & 0x8) == 0x8 && (r & 0x8) == 0x8){
			carry = 1;
		}*/
		ct = (st & 0x3f) - carry - ((r >> 4)&0x3f);
		c = c | ((ct&0x3f) << 4);

		//compute M[24-19]
		carry = 0;
		m = LL(c, 15);
		m = m + t;
		m = (m >> 19) & 0x3f;
		//compute S[15-10]
		st = m;
		s = s | (st << 10);
		//compute C[15-10]
		//check the carry to the 10th bit from the 9th bit.
		if (getBit(s, 9) == 0 && (getBit(c, 9) | getBit(r, 9)) == 1){
			carry = 1;
		}
		/*if (s & 0x200 == 0 && ((c & 0x200) | (r & 0x200) == 0x200)){
			carry == 1;
		}*/
		else if (getBit(s, 9) == 1 && getBit(c, 9) == 1 && getBit(r, 9) == 1){
			carry = 1;
		}
		/*else if ((s & 0x200) == 0x200 && (c & 0x200) == 0x200 && (r & 0x200) == 0x200){
			carry = 1;
		}*/
		ct = (st & 0x3f) - carry - ((r >> 10) & 0x3f);
		c = c | ((ct & 0x3f) << 10);

		//compute M[30-25]
		carry = 0;
		m = LL(c, 15);
		m = m + t;
		m = (m >> 25) & 0x3f;
		//compute S[21-16]
		st = m;
		s = s | (st << 16);
		//compute C[21-16]
		//check the carry to the 15th bit from the 14th bit.
		if (getBit(s, 15) == 0 && getBit(c, 15) | getBit(r, 15) == 1){
			carry = 1;
		}
		/*if (s & 0x8000 == 0 && ((c & 0x8000) | (r & 0x8000) == 0x8000)){
			carry == 1;
		}*/
		else if (getBit(s, 15) == 1 && getBit(c, 15) == 1 && getBit(r, 15) == 1){
			carry = 1;
		}
		/*else if ((s & 0x8000) == 0x8000 && (c & 0x8000) == 0x8000 && (r & 0x8000) == 0x8000){
			carry = 1;
		}*/
		ct = (st & 0x3f) - carry - ((r >> 16) & 0x3f);
		c = c | ((ct & 0x3f) << 16);

		//filtering
		if ((i & 0x1f) == ((c >> 17) & 0x1f)){
			c = c | ((i >> 5) << 22);//C[22]=i[5] and C[23]=i[6]
			if (LL(c, 15) + t == LL(c + r, 9)){
				//cout << hex << c << endl;
				value[length] = c;
				length++;
				//cout << "Solution exists!" << endl;
				//flag = true;
				//return c;
			}
		}
	}
	//flag = false;
	//return c;
}

class RIPEMD160{
private:
	unsigned int word[WORD_NUM];//message word
	unsigned int LIV[5];//IV in the left branch
	unsigned int RIV[5];//IV in the right branch
	unsigned int LTemp[36];//Internal state in the left branch
	unsigned int RTemp[36];//Internal state in the right branch
	unsigned int LDiff[36];//The difference of the internal state in the left branch
	unsigned int RDiff[36];//The difference of the internal state in the right branch
	unsigned int LC[5];//constants in the left branch
	unsigned int RC[5];//constants values in the right branch
	unsigned char LS[5][16];//rotation values in the left branch
	unsigned char RS[5][16];//rotation values in the right branch
	unsigned char LI[5][16];//word permutation in the left branch
	unsigned char RI[5][16];//word permutation in the right branch

	unsigned int *validRT0;//valid RTemp[0]
public:
	RIPEMD160();
	~RIPEMD160();
	void initialize();
	void initializeAfterComputingHash();
	void computeHash(int start,int steps);

	void merge(bool isUntilTheFirstStep);
	inline unsigned int getRand();
	inline unsigned int computeX(int index);//compute backward the internal state in the left branch

	int verifyM3();//check whether M3 is valid, that's whether it can make the differential path held.(Right branch)
	void testVerifyingProbability();//test the probability that M3 makes the differential path held. (Right Branch)

	bool verifyM14();//check whether M14 is valid, that's whether it can make the differential path held. (Left branch)
	void testVerifyingProbabilityLeft();//test the probability that M14 makes the differential path held. (Left Branch)

	bool verifyX5X4();//check the influence of X5 and X4 on the difference while computing backward.
	bool verifyX3();//check the influence of X3 on the difference while computing backward.
	bool verifyY1Y0();//generate valid RTemp[0],that's valid M2 and Y1.

	bool verifyRIV3();//check whether RIV[3](Y-1) is valid

	void verifyResult();
};

RIPEMD160::RIPEMD160(){
	//Initialization of IV
	LIV[0] = RIV[0] = 0XC059D148;
	LIV[1] = RIV[1] = 0X7C30F4B8;
	LIV[2] = RIV[2] = 0X1D840C95;
	LIV[3] = RIV[3] = 0X98BADCFE;
	LIV[4] = RIV[4] = 0XEFCDAB89;

	//Initialization of constants in the left branch.
	LC[0] = 0X0;
	LC[1] = 0X5A827999;
	LC[2] = 0X6ED9EBA1;
	LC[3] = 0X8F1BBCDC;
	LC[4] = 0XA953FD4E;

	//Initialization of constants in the right branch.
	RC[0] = 0X50A28BE6;
	RC[1] = 0X5C4DD124;
	RC[2] = 0X6D703EF3;
	RC[3] = 0X7A6D76E9;
	RC[4] = 0X0;

	unsigned char LST[5][16] = { 11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
								7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
								11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
								11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
								9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6 };

	unsigned RST[5][16] = { 8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
						   9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
						   9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
						   15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
						   8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11 };

	unsigned int LIT[5][16] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
								7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
								3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
								1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
								4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13 };

	unsigned int RIT[5][16] = { 5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
								6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
								15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
								8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
								12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11 };

	for (int i = 0; i != 5; i++){
		for (int j = 0; j != 16; j++){
			LS[i][j] = LST[i][j];//Initialization of rotation values in the left branch.
			RS[i][j] = RST[i][j];//Initialization of rotation values in the right branch.
			LI[i][j] = LIT[i][j];//Initialization of word permutation in the left branch.
			RI[i][j] = RIT[i][j];//Initialization of word permutation in the right branch.
		}
	}
	validRT0 = new unsigned int[VALIDNUM];
}

RIPEMD160::~RIPEMD160(){
	delete[]validRT0;
}

//Compute the hash value according to the start step and the steps
void RIPEMD160::computeHash(int start,int steps){
	cout << "M7:" << hex << word[7] << endl;
	for (int i = start; i < steps; i++){
		if (i < 5){
			switch (i){
			case 0:
				LTemp[0] = LL(LIV[1], 10) + LL(LL(LIV[0], 10) + XOR(LIV[4], LIV[3], LL(LIV[2], 10)) + word[LI[0][0]] + LC[0], LS[0][0]);
				RTemp[0] = LL(RIV[1], 10) + LL(LL(RIV[0], 10) + ONX(RIV[4], RIV[3], LL(RIV[2], 10)) + word[RI[0][0]] + RC[0], RS[0][0]);
				break;
			case 1:
				LTemp[1] = LL(LIV[2], 10) + LL(LL(LIV[1], 10) + XOR(LTemp[0], LIV[4], LL(LIV[3], 10)) + word[LI[0][1]] + LC[0], LS[0][1]);
				RTemp[1] = LL(RIV[2], 10) + LL(LL(RIV[1], 10) + ONX(RTemp[0], RIV[4], LL(RIV[3], 10)) + word[RI[0][1]] + RC[0], RS[0][1]);
				break;
			case 2:
				LTemp[2] = LL(LIV[3], 10) + LL(LL(LIV[2], 10) + XOR(LTemp[1], LTemp[0], LL(LIV[4], 10)) + word[LI[0][2]] + LC[0], LS[0][2]);
				RTemp[2] = LL(RIV[3], 10) + LL(LL(RIV[2], 10) + ONX(RTemp[1], RTemp[0], LL(RIV[4], 10)) + word[RI[0][2]] + RC[0], RS[0][2]);
				break;
			case 3:
				LTemp[3] = LL(LIV[4], 10) + LL(LL(LIV[3], 10) + XOR(LTemp[2], LTemp[1], LL(LTemp[0], 10)) + word[LI[0][3]] + LC[0], LS[0][3]);
				RTemp[3] = LL(RIV[4], 10) + LL(LL(RIV[3], 10) + ONX(RTemp[2], RTemp[1], LL(RTemp[0], 10)) + word[RI[0][3]] + RC[0], RS[0][3]);
				break;
			case 4:
				LTemp[4] = LL(LTemp[0], 10) + LL(LL(LIV[4], 10) + XOR(LTemp[3], LTemp[2], LL(LTemp[1], 10)) + word[LI[0][4]] + LC[0], LS[0][4]);
				RTemp[4] = LL(RTemp[0], 10) + LL(LL(RIV[4], 10) + ONX(RTemp[3], RTemp[2], LL(RTemp[1], 10)) + word[RI[0][4]] + RC[0], RS[0][4]);
				break;
			}
		}
		else if (i < 16){
			LTemp[i] = LL(LTemp[i - 4], 10) + LL(LL(LTemp[i - 5], 10) + XOR(LTemp[i - 1], LTemp[i - 2], LL(LTemp[i - 3], 10)) + word[LI[0][i]] + LC[0], LS[0][i]);
			RTemp[i] = LL(RTemp[i - 4], 10) + LL(LL(RTemp[i - 5], 10) + ONX(RTemp[i - 1], RTemp[i - 2], LL(RTemp[i - 3], 10)) + word[RI[0][i]] + RC[0], RS[0][i]);
		}
		else if (i < 32){
			LTemp[i] = LL(LTemp[i - 4], 10) + LL(LL(LTemp[i - 5], 10) + IFX(LTemp[i - 1], LTemp[i - 2], LL(LTemp[i - 3], 10)) + word[LI[1][i - 16]] + LC[1], LS[1][i - 16]);
			RTemp[i] = LL(RTemp[i - 4], 10) + LL(LL(RTemp[i - 5], 10) + IFZ(RTemp[i - 1], RTemp[i - 2], LL(RTemp[i - 3], 10)) + word[RI[1][i - 16]] + RC[1], RS[1][i - 16]);
		}
		else if (i < 48){
			LTemp[i] = LL(LTemp[i - 4], 10) + LL(LL(LTemp[i - 5], 10) + ONZ(LTemp[i - 1], LTemp[i - 2], LL(LTemp[i - 3], 10)) + word[LI[2][i - 32]] + LC[2], LS[2][i - 32]);
			RTemp[i] = LL(RTemp[i - 4], 10) + LL(LL(RTemp[i - 5], 10) + ONZ(RTemp[i - 1], RTemp[i - 2], LL(RTemp[i - 3], 10)) + word[RI[2][i - 32]] + RC[2], RS[2][i - 32]);
		}
		else if (i < 64){
			LTemp[i] = LL(LTemp[i - 4], 10) + LL(LL(LTemp[i - 5], 10) + IFZ(LTemp[i - 1], LTemp[i - 2], LL(LTemp[i - 3], 10)) + word[LI[3][i - 48]] + LC[3], LS[2][i - 48]);
			RTemp[i] = LL(RTemp[i - 4], 10) + LL(LL(RTemp[i - 5], 10) + IFX(RTemp[i - 1], RTemp[i - 2], LL(RTemp[i - 3], 10)) + word[RI[3][i - 48]] + RC[3], RS[2][i - 48]);
		}
		else{
			LTemp[i] = LL(LTemp[i - 4], 10) + LL(LL(LTemp[i - 5], 10) + ONX(LTemp[i - 1], LTemp[i - 2], LL(LTemp[i - 3], 10)) + word[LI[4][i - 64]] + LC[4], LS[2][i - 64]);
			RTemp[i] = LL(RTemp[i - 4], 10) + LL(LL(RTemp[i - 5], 10) + XOR(RTemp[i - 1], RTemp[i - 2], LL(RTemp[i - 3], 10)) + word[RI[4][i - 64]] + RC[4], RS[2][i - 64]);
		}
	}
	//cout << hex << LTemp[steps - 1] << " " << RTemp[steps - 1] << endl;
	int ivIndex = start;
	for (; ivIndex < 5; ivIndex++){
		cout << "X" << dec << ivIndex-4 << ":";
		cout << hex << setw(8) << LIV[ivIndex] << "\t";
		cout << "Y" << dec << ivIndex - 4 << ":";
		cout<< hex << setw(8) << RIV[ivIndex] << endl;
	}
	for (unsigned int i = start; i < steps; i++){
		cout <<"X"<< dec << i+1 << ":";
		cout << hex << setw(8) << LTemp[i] << "\t" << "Y" << dec << i << ":" <<hex<< setw(8) << RTemp[i] << endl;
	}
	cout << endl;
}

//Use the modified starting point
void RIPEMD160::initialize(){
	word[1] = 0x67dbd0a9;
	word[3] = 0x0827ae72;
	word[4] = 0x5cd30b65;
	word[6] = 0x651c397d;
	word[8] = 0x050ff865;
	word[10] = 0xa9f94c09;
	word[11] = 0x509bf856;
	word[12] = 0x0588c327;
	word[13] = 0x86671566;
	word[15] = 0xc3349b51;
	//new added
	word[9] = 0x471fba32;//For the left branch, there won't exist contradictions while computing backward when fixing M9=0x471fba32.
	LTemp[3] = 0x204c4371;//After pre-computing one valid M9, a pair of (LTemp[4],LTemp[3]) can be obtained.
	LTemp[4] = 0xcd4c2bb4;

	/*/check the result:
	word[0] = 0x678c8c36;
	word[1] = 0x67dbd0a9;
	word[2] = 0x5293b823;
	word[3] = 0xd90c1aa9;
	word[4] = 0x5cd30b65;
	word[5] = 0x13d3dff6;
	word[6] = 0x651c397d;
	word[7] = 0x794a60c6;
	word[8] = 0x50ff865;
	word[9] = 0x471fba32;
	word[10] = 0xa9f94c09;
	word[11] = 0x509bf856;
	word[12] = 0x588c327;
	word[13] = 0x86671566;
	word[14] = 0xee8e443e;
	word[15] = 0xc3349b51;

	LIV[0] = 0xd055ce6;
	LIV[1] = 0xdf979ac7;
	LIV[2] = 0xae4836b3;
	LIV[3] = 0x57b6f5fb;
	LIV[4] = 0x6b9ec934;

	RIV[0] = LIV[0];
	RIV[1] = LIV[1];
	RIV[2] = LIV[2];
	RIV[3] = LIV[3];
	RIV[4] = LIV[4];
	computeHash(0, 14);
	word[7] = word[7] + 0x40008010;
	computeHash(0, 14);
	system("pause");
	/*/

	LTemp[5] = 0xdcad7259;
	LTemp[6] = 0x4d74d3eb;
	LTemp[7] = 0x30fff7ff;
	LTemp[8] = 0x30b13b28;
	LTemp[9] = 0xb3b290cb;

	RTemp[1] = 0x82fc4630;
	RTemp[2] = 0x7ac05221;
	RTemp[3] = 0x9c316390;
	RTemp[4] = 0x5670307d;
	RTemp[5] = 0x11f53fff;

	//used for checking the validity of M3
	RTemp[9] = 0x780c804e;
	RTemp[10] = 0x7ab70b85;
	RTemp[11] = 0x9ac6d81e;
	RTemp[12] = 0xe1f41e57;
	RTemp[13] = 0x488607d0;

	//used for checking the validity of M14
	LTemp[10] = 0x95f6a45c;
	LTemp[11] = 0x1cd0000d;
	LTemp[12] = 0xf87f4314;
	LTemp[13] = 0x9336ea12;

	//The difference in the right branch
	RDiff[1] = 0;
	RDiff[2] = 0x01002080;
	RDiff[3] = 0x01040008;
	RDiff[4] = 0x80010000;
	RDiff[5] = 0x80063FC2;
	//RDiff[6] = 0x01040008;

	RDiff[9] = 0x17E00008;
	RDiff[10] = 0x15019402;
	RDiff[11] = 0x00208001;
	RDiff[12] = 0x45000200;
	RDiff[13] = 0x00400820;

	//The difference in the left branch
	LDiff[7] = 0x02FFEF7F;
	LDiff[8] = 0x3B47F013;
	LDiff[9] = 0x04FA8308;
	LDiff[10] = 0x23CCD000;
	LDiff[11] = 0x042FFFE0;

	//verifyX5X4();//To compute one valid M9
	//system("pause");
	verifyY1Y0();//set all possible values to RTemp[0]
}

void RIPEMD160::initializeAfterComputingHash(){
	word[1] = 0x67dbd0a9;
	word[3] = 0x0827ae72;
	word[4] = 0x5cd30b65;
	word[6] = 0x651c397d;
	word[8] = 0x050ff865;
	word[10] = 0xa9f94c09;
	word[11] = 0x509bf856;
	word[12] = 0x0588c327;
	word[13] = 0x86671566;
	word[15] = 0xc3349b51;
	//new added
	word[9] = 0x471fba32;//For the left branch, there won't exist contradictions while computing backward when fixing M9=0x471fba32.
	LTemp[3] = 0x204c4371;//After pre-computing one valid M9, a pair of (LTemp[4],LTemp[3]) can be obtained.
	LTemp[4] = 0xcd4c2bb4;

	LTemp[5] = 0xdcad7259;
	LTemp[6] = 0x4d74d3eb;
	LTemp[7] = 0x30fff7ff;
	LTemp[8] = 0x30b13b28;
	LTemp[9] = 0xb3b290cb;

	RTemp[1] = 0x82fc4630;
	RTemp[2] = 0x7ac05221;
	RTemp[3] = 0x9c316390;
	RTemp[4] = 0x5670307d;
	RTemp[5] = 0x11f53fff;

	//used for checking the validity of M3
	RTemp[9] = 0x780c804e;
	RTemp[10] = 0x7ab70b85;
	RTemp[11] = 0x9ac6d81e;
	RTemp[12] = 0xe1f41e57;
	RTemp[13] = 0x488607d0;

	//used for checking the validity of M14
	LTemp[10] = 0x95f6a45c;
	LTemp[11] = 0x1cd0000d;
	LTemp[12] = 0xf87f4314;
	LTemp[13] = 0x9336ea12;

	//The difference in the right branch
	RDiff[1] = 0;
	RDiff[2] = 0x01002080;
	RDiff[3] = 0x01040008;
	RDiff[4] = 0x80010000;
	RDiff[5] = 0x80063FC2;
	//RDiff[6] = 0x01040008;

	RDiff[9] = 0x17E00008;
	RDiff[10] = 0x15019402;
	RDiff[11] = 0x00208001;
	RDiff[12] = 0x45000200;
	RDiff[13] = 0x00400820;

	//The difference in the left branch
	LDiff[7] = 0x02FFEF7F;
	LDiff[8] = 0x3B47F013;
	LDiff[9] = 0x04FA8308;
	LDiff[10] = 0x23CCD000;
	LDiff[11] = 0x042FFFE0;
}

//Get random value
inline unsigned int RIPEMD160::getRand(){
	unsigned int num1 = 0,num2=0;
	num1 = rand() & 0xffff;
	num2 = rand() & 0xffff;
	return (num1 << 16) | num2;
}

//compute backward the internal state in the left branch
inline unsigned int RIPEMD160::computeX(int index){
	//It is simple if we only take the first round into consideration
	unsigned int t=0;
	if (index >= 0){
		t = LTemp[index + 5] - LL(LTemp[index + 1], 10);
		t = RR(t, LS[0][index + 5]) - XOR(LTemp[index + 4], LTemp[index + 3], LL(LTemp[index + 2], 10)) - LC[0] - word[LI[0][index + 5]];
		t = RR(t, 10);
	}
	return t;
}

//Merge both branches
void RIPEMD160::merge(bool isUntilTheFirstStep){
	//Set random values to word[7] and word[9]
	unsigned int test = 0;
	bool find = false;
	unsigned long long total[3] = { 0, 0, 0};//used for counting the total running time of the program.
	unsigned long long increase[3] = { 0, 0, 0 };//used for counting the increasing running time of the program compared with that of last time

	unsigned int value[5];//store the solution.
	int length = 0;//The number of the solutions

	srand(time(NULL));
	word[7] = getRand();

	LTemp[2] = computeX(2);//word[7] should be known.
	LTemp[1] = computeX(1);

	unsigned int diff2 = RTemp[2] ^ RDiff[2];

	unsigned int index = 0;
	unsigned int count = 1;
	unsigned int diff = RTemp[2] - LTemp[2];
	cout << "Randomly generate m7! " << endl;
	cout << hex << "m_7:" << word[7] << endl;

	ofstream runningTime("runningTime.txt", ios::app);

	while (!find){
		total[0]++;//used for counting the running time of the program. Exception:2^{33+1.5}=2^{34.5}
		//set values to RTemp[0]
		RTemp[0] = validRT0[index];//Y-1 and Y0 will have no influence on the difference of Y4,Y5. Besides, the condition on Y1 is satisfied.

		//compute word[2] ,RIV[4],LIV[4]
		word[2] = RR(RTemp[5] - LL(RTemp[1], 10), RS[0][5]) - (ONX(RTemp[4], RTemp[3], LL(RTemp[2], 10)) + LL(RTemp[0], 10) + RC[0]);
		RIV[4] = RR(RTemp[4] - LL(RTemp[0], 10), RS[0][4]) - (ONX(RTemp[3], RTemp[2], LL(RTemp[1], 10)) + word[RI[0][4]] + RC[0]);//word[9] is known
		RIV[4] = RR(RIV[4], 10);
		LIV[4] = RIV[4];

		//compute LTemp[0]
		LTemp[0] = LTemp[4] - LL(LL(LIV[4], 10) + XOR(LTemp[3], LTemp[2], LL(LTemp[1], 10)) + word[LI[0][4]] + LC[0], LS[0][4]);//word[4] is konwn
		//compute word[5]
		word[5] = RR(LTemp[5] - LL(LTemp[1], 10), LS[0][5]) - (LTemp[0] + XOR(LTemp[4], LTemp[3], LL(LTemp[2], 10)) + LC[0]);
		LTemp[0] = RR(LTemp[0], 10);

		//Based on LIV[3]=RIV[3] and LIV[2]=RIV[2],compute word[0] and word[3], an equation system is needed.
		unsigned int T0, T1, T2, T4, T5, T6, T7, T8, S, X;//Parameters for constructing the equation system.
		T0 = RR(LTemp[3] - LL(LIV[4], 10), 12) - (LC[0] + XOR(LTemp[2], LTemp[1], LL(LTemp[0], 10)));
		T1 = LTemp[2] - T0;
		T2 = XOR(LTemp[1], LTemp[0], LL(LIV[4], 10)) + LC[0] + word[2];

		T4 = RR(RTemp[3] - LL(RIV[4], 10), 11) - (ONX(RTemp[2], RTemp[1], LL(RTemp[0], 10)) + RC[0]);
		T5 = ONX(RTemp[1], RTemp[0], LL(RIV[4], 10)) + RC[0] + word[7];
		T8 = T5 - T2;

		solveEqation(T8, diff, length, value);//solve the equation system
		for (int i = 0; i < length; i++){
			total[1]++;//Expection:2^{32+1}=2^{33}

			X = value[i];
			S = LL(X, 15);
			word[3] = S - T1;
			word[0] = word[3] + T4 - T0;

			LIV[3] = RR(LTemp[3] - LL(LIV[4], 10), LS[0][3]) - (word[LI[0][3]] + LC[0] + XOR(LTemp[2], LTemp[1], LL(LTemp[0], 10)));

			if (RR(RTemp[2] - LIV[3], 9) + 0x40008010 == RR(diff2 - LIV[3], 9)){//chech the validity of LIV[3].(That's Y-2) Pro=2^{-1}
				LIV[3] = RR(LIV[3], 10);
				RIV[3] = LIV[3];
				total[2]++;//Expection:2^{32+1}=2^{32}

				LIV[2] = RR(LTemp[2] - LL(LIV[3], 10), LS[0][2]) - (word[LI[0][2]] + LC[0] + XOR(LTemp[1], LTemp[0], LL(LIV[4], 10)));
				LIV[2] = RR(LIV[2], 10);
				RIV[2] = LIV[2];

				//compute word[14] according to the equation LIV[1]=RIV[1]
				LIV[1] = RR(LTemp[1] - LL(LIV[2], 10), LS[0][1]) - (word[LI[0][1]] + LC[0] + XOR(LTemp[0], LIV[4], LL(LIV[3], 10)));
				RIV[1] = LIV[1];

				word[14] = RR(RTemp[1] - LL(RIV[2], 10), RS[0][1]) - (RC[0] + ONX(RTemp[0], RIV[4], LL(RIV[3], 10)) + RIV[1]);
				LIV[1] = RR(LIV[1], 10);
				RIV[1] = RR(RIV[1], 10);

				///////Do testing,compute until X_{-3} and Y_{-3}
				if (isUntilTheFirstStep == false){
					computeHash(1, 14);
					word[7] = word[7] + 0x40008010;
					computeHash(1, 14);
					initializeAfterComputingHash();
					word[7] = word[7] - 0x40008010;
					LTemp[2] = computeX(2);
					LTemp[1] = computeX(1);
					system("pause");
				}
				///////

				//check whether LIV[0] is equal to RIV[0]
				LIV[0] = RR(LTemp[0] - LL(LIV[1], 10), LS[0][0]) - (word[LI[0][0]] + LC[0] + XOR(LIV[4], LIV[3], LL(LIV[2], 10)));
				RIV[0] = RR(RTemp[0] - LL(RIV[1], 10), RS[0][0]) - (word[RI[0][0]] + RC[0] + ONX(RIV[4], RIV[3], LL(RIV[2], 10)));

				if (LIV[0] == RIV[0]){//Pro:2^{-32}
					find = true;

					LIV[0] = RR(LIV[0], 10);
					RIV[0] = LIV[0];
					///////Do testing,compute until X_{-3} and Y_{-3}
					///////

					cout << "IV:" << endl;
					for (int i = 0; i < 5; i++){
						cout << hex << LIV[i] << endl;
					}

					cout << "word:" << endl;
					for (int i = 0; i < 16; i++){
						cout << hex << word[i] << endl;
					}
					cout << endl;

					cout << "Running times:";
					for (int i = 0; i < 3; i++){
						cout << hex << "total[" << i << "]:" << total[i] << endl;
					}
					cout << "--------------------------------" << endl << endl;

					ofstream outFile("word_hex.txt", ios::app);
					for (int i = 0; i < 16; i++){
						outFile << "word[" << i << "]: " << hex << word[i] << endl;
						//cout << "word[" << i << "]: " << hex << word[i] << endl;
					}

					for (int i = 0; i < 3; i++){
						//cout << hex << "total[" << i << "]:" << total[i] << endl;
						outFile << hex << "total[" << i << "]:" << total[i] << endl;
					}

					for (int i = 0; i < 5; i++){
						outFile << "IV[" << i << "]:" << hex << LIV[i] << endl;
						//cout << "IV[" << i << "]:" << hex << LIV[i] << endl;
					}

					outFile << "--------------------------------" << endl << endl;
					outFile.close();
				}
			}
		}

		index++;
		//cout << hex << index << endl;
		if (index == VALIDNUM){
			index = 0;
			word[7] = getRand();

			cout << hex << count << ":" <<endl;
			cout << hex << "Total Running times until Step 6:" << total[1] << endl;
			cout << hex << "Total Running times until Step 7:" << total[2] << endl;
			cout << hex << "Running times until Step 6 for " << VALIDNUM << " computations: " << total[1] - increase[1] << endl;
			cout << hex << "Running times until Step 7 for " << VALIDNUM << " computations: " << total[2] - increase[2] << endl;
			cout << endl;

			//store the running times
			runningTime << hex << count << ":" << endl;
			runningTime << hex << "Total Running times until Step 6:" << total[1] << endl;
			runningTime << hex << "Total Running times until Step 7:" << total[2] << endl;
			runningTime << hex << "Running times until Step 6 for " << VALIDNUM << " computations: " << total[1] - increase[1] << endl;
			runningTime << hex << "Running times until Step 7 for " << VALIDNUM << " computations: " << total[2] - increase[2] << endl;
			runningTime << endl;

			increase[1] = total[1];
			increase[2 ] = total[2];

			count++;
			cout << hex << "m_7:" << word[7] << endl;

			runningTime << hex << "m_7:" << word[7] << endl;

			LTemp[2] = computeX(2);
			LTemp[1] = computeX(1);
			diff = RTemp[2] - LTemp[2];
		}
	}
	runningTime.close();
}

//check whether M3 is valid, that's whether it can make the differential path held.(Right branch)
int RIPEMD160::verifyM3(){

	unsigned int count[5] = { 0 };

	unsigned int tc[16] = { 0 };
	unsigned int index = 0;

	unsigned int tc16[8] = { 0 };
	unsigned int index16 = 0;

	unsigned int tc17[8] = { 0 };
	unsigned int index17 = 0;

	unsigned int t1, t2;

	//Compute RTemp[14-19]
	unsigned int RDT[19];//store RTemp[i]' (i=9,10,11,12,13,14,15,16,17,18)
	int finish = 19;

	//Correct the values Y10-14（RTemp[9-13]）,recompute Y15-20，that's RTemp[14-19]
	for (int i = 9; i < 14; i++){
		RDT[i] = RTemp[i] ^ RDiff[i];
	}

	for (unsigned int k = 0; k < 0xffffffff; k++){
		word[3] =k;

		RTemp[14] = LL(RTemp[10], 10) + LL(LL(RTemp[9], 10) + ONX(RTemp[13], RTemp[12], LL(RTemp[11], 10)) + word[RI[0][14]] + RC[0], RS[0][14]);
		RTemp[15] = LL(RTemp[11], 10) + LL(LL(RTemp[10], 10) + ONX(RTemp[14], RTemp[13], LL(RTemp[12], 10)) + word[RI[0][15]] + RC[0], RS[0][15]);
		for (int i = 16; i < finish; i++){
			RTemp[i] = LL(RTemp[i - 4], 10) + LL(LL(RTemp[i - 5], 10) + IFZ(RTemp[i - 1], RTemp[i - 2], LL(RTemp[i - 3], 10)) + word[RI[1][i - 16]] + RC[1], RS[1][i - 16]);
		}

		RDT[14] = LL(RDT[10], 10) + LL(LL(RDT[9], 10) + ONX(RDT[13], RDT[12], LL(RDT[11], 10)) + word[RI[0][14]] + RC[0], RS[0][14]);//RTemp[14]

		if (getBit(RDT[14], 5) == 0 && getBit(RDT[14], 20) == 0 && getBit(RDT[14], 26) == 0 && RTemp[14] == RDT[14] + 0x4100020){
			//
			//count[0]++;

			//////RDT[14]22,19,11,2]
			index = 0;
			if (getBit(RDT[14], 2) == 1){
				index = index + 1;
			}
			if (getBit(RDT[14], 11) == 1){
				index = index + 2;
			}
			if (getBit(RDT[14], 19) == 1){
				index = index + 4;
			}
			if (getBit(RDT[14], 22) == 1){
				index = index +8;
			}
			tc[index]++;
			/////

			//cout << getBit(RDT[14], 5) << " " << getBit(RDT[14], 20) << " " << getBit(RDT[14], 26) << endl;
			RDT[15] = LL(RDT[11], 10) + LL(LL(RDT[10], 10) + ONX(RDT[14], RDT[13], LL(RDT[12], 10)) + word[RI[0][15]] + RC[0], RS[0][15]);//RTemp[15]
			if (RDT[15] == RTemp[15]){//Test RTemp[15]
				count[1]++;

				/////
				index16 = 0;
				if (getBit(RDT[14], 0) == getBit(RDT[15], 0)){
					index16 = index16 + 1;
				}
				if (getBit(RDT[14], 15) == getBit(RDT[15], 15)){
					index16 = index16 + 2;
				}
				if (getBit(RDT[14], 21) == getBit(RDT[15], 21)){
					index16 = index16 + 4;
				}
				tc16[index16]++;
				////////

				for (int i = 16; i < finish; i++){
					RDT[i] = LL(RDT[i - 4], 10) + LL(LL(RDT[i - 5], 10) + IFZ(RDT[i - 1], RDT[i - 2], LL(RDT[i - 3], 10)) + word[RI[1][i - 16]] + RC[1], RS[1][i - 16]);//RTemp[16-19]

					/////
					index17 = 0;
					if (i == 17 && RDT[16] == RTemp[16]){//Conditions on RDT[15] and RDT[16] (15,30)
						if (getBit(RDT[15], 15) == getBit(RDT[16], 15)){
							index17 = index17 + 1;
						}
						if (getBit(RDT[15], 30) == getBit(RDT[16], 30)){
							index17 = index17 + 2;
						}
						tc17[index17]++;
					}
					////////

					if (RDT[i] == RTemp[i]){//Test RTemp[16-18]
						count[i - 14]++;
					}
					else{
						break;
					}
				}

			}
			//return 5;
		}
	}
	for (int i = 0; i < 5; i++){
		cout << setw(8) << hex << count[i] << " ";
	}
	cout << endl;

	for (unsigned int i = 0; i < 16; i++){
		cout << setw(8) << hex <<i<<": "<< tc[i] <<endl;
	}
	cout << endl;

	for (unsigned int i = 0; i < 8; i++){
		cout << setw(8) << hex << i << ": " << tc16[i] << endl;
	}
	cout << endl;

	for (unsigned int i = 0; i < 4; i++){
		cout << setw(8) << hex << i << ": " << tc17[i] << endl;
	}
	cout << endl;

	return 0;
}

//test the probability that M3 makes the differential path held. (Right Branch)
void RIPEMD160::testVerifyingProbability(){
	//unsigned int count = 0;
	unsigned int end = 0xffffffff;
	srand(time(NULL));
	word[7] = getRand();

	unsigned int count[5] = { 0 };

	//ofstream outFile("m3.txt");
	for (unsigned int i = 0; i < end; i++){
		word[3] = i;
		/*if (verifyM3()){
			count++;
		}*/
		int end1 = verifyM3();
		for (int j = 0; j < end1; j++){
			count[j]++;
		}
	}
	//outFile.close();
	//cout <<dec<< count << endl;
	for (int i = 0; i < 5; i++){
		cout <<setw(8)<< hex << count[i] <<" ";
	}
	cout << endl;
}

//check whether M14 is valid, that's whether it can make the differential path held. (Left branch)
bool RIPEMD160::verifyM14(){
	unsigned int LT[2];
	unsigned int count1 = 0, count2 = 0,count3=0;
	for (unsigned int i = 0; i < 0xffffffff; i++){
		word[14] = i;

		LTemp[14] = LL(LTemp[10], 10) + LL(LL(LTemp[9], 10) + XOR(LTemp[13], LTemp[12], LL(LTemp[11], 10)) + word[LI[0][14]] + LC[0], LS[0][14]);
		LTemp[15] = LL(LTemp[11], 10) + LL(LL(LTemp[10], 10) + XOR(LTemp[14], LTemp[13], LL(LTemp[12], 10)) + word[LI[0][15]] + LC[0], LS[0][15]);
		LT[0] = LTemp[14];
		LT[1] = LTemp[15];

		LTemp[9] = LTemp[9] ^ LDiff[9];
		LTemp[10] = LTemp[10] ^ LDiff[10];
		LTemp[11] = LTemp[11] ^ LDiff[11];

		LTemp[14] = LL(LTemp[10], 10) + LL(LL(LTemp[9], 10) + XOR(LTemp[13], LTemp[12], LL(LTemp[11], 10)) + word[LI[0][14]] + LC[0], LS[0][14]);
		LTemp[15] = LL(LTemp[11], 10) + LL(LL(LTemp[10], 10) + XOR(LTemp[14], LTemp[13], LL(LTemp[12], 10)) + word[LI[0][15]] + LC[0], LS[0][15]);

		if (LTemp[14] == LT[0]){
			count1++;//0x3ca85f7f
		}
		if (LTemp[15] == LT[1]){
			count2++;//0x1020000
		}

		if (LTemp[14] == LT[0] && LTemp[15] == LT[1]){
			count3++;
		}
		LTemp[9] = LTemp[9] ^ LDiff[9];//Reset
		LTemp[10] = LTemp[10] ^ LDiff[10];
		LTemp[11] = LTemp[11] ^ LDiff[11];
	}
	cout << hex << count1 << " " << count2 <<" "<<count3<< endl;
	return 0;
}

//test the probability that M14 makes the differential path held. (Left Branch)
void RIPEMD160::testVerifyingProbabilityLeft(){
	unsigned int count = 0;
	for (unsigned int i = 0; i < 0xffffffff; i++){
		word[14] = i;
		if (verifyM14()){
			count++;
		}
	}
	cout << hex << count << endl;
}

//check the influence of X3(LTemp[2]) on the difference while computing backward.
bool RIPEMD160::verifyX3(){
	LTemp[7] = LTemp[7] ^ LDiff[7];
	word[7] = word[7] + 0x40008010;
	unsigned int t2 = computeX(2);
	LTemp[7] = LTemp[7] ^ LDiff[7];//reset
	word[7] = word[7] - 0x40008010;
	if (t2 == LTemp[2]){
		return true;
	}
	return false;
}

//check the influence of X5 and X4 on the difference while computing backward.
bool RIPEMD160::verifyX5X4(){
	unsigned int t4, t3,count1=0,count2=0;
	unsigned int end = 0xffffffff;

	unsigned int diff1,diff2,diff8,diff7,tmp;
	diff8 = LTemp[8] ^ LDiff[8];
	diff7 = LTemp[7] ^ LDiff[7];

	for (unsigned int i = 0; i < end; i++){
		word[9] = i;

		LTemp[4] = computeX(4);
		LTemp[3] = computeX(3);

		diff2 = XOR(diff7, LTemp[6], LL(LTemp[5], 10)) - XOR(LTemp[7], LTemp[6], LL(LTemp[5], 10));
		diff1 = RR(diff8 - LL(LTemp[4], 10), 11) - RR(LTemp[8] - LL(LTemp[4], 10), 11);

		if (diff1==diff2){//check the validity of LTemp[3], that's X4
			count1++;
			tmp = RR(diff7 - LL(LTemp[3], 10), 9) - RR(LTemp[7] - LL(LTemp[3], 10), 9);

			if (tmp == 0x40008010){//check the validity of LTemp[2], that's X3
				srand(time(NULL));
				word[7] = getRand();

				LTemp[4] = computeX(4);
				LTemp[3] = computeX(3);
				LTemp[2] = computeX(2);
				LTemp[1] = computeX(1);

				word[7] = word[7] + 0x40008010;
				computeHash(6, 11);
				word[7] = word[7] - 0x40008010;
				computeHash(6, 11);

				cout << hex << i << endl;
				system("pause");
				//validM9[count2] = i;
				cout << "One valid M9:" << hex << i << endl;
				cout << "LTemp[4]: " << hex << LTemp[4] << endl;
				cout << "LTemp[3]: " << hex << LTemp[3] << endl;
				system("pause");
				count2++;
			}
		}
	}
	cout << "Count1:" << hex << count1 << endl;
	cout <<"Count2:"<< hex  <<  count2 << endl;
	return true;
}

//generate valid RTemp[0],that's valid M2 and Y1.
//the number of valid RTemp[0]:fcf2100
bool RIPEMD160::verifyY1Y0(){
	unsigned int t1 = 0xfffc0008, t0 = 0x1002080;//t1 for Y1,t0 for Y0
	unsigned int diff5,diff4, diff3, diff2;
	diff5 = RTemp[5] ^ RDiff[5];
	diff4 = RTemp[4] ^ RDiff[4];
	diff3 = RTemp[3] ^ RDiff[3];
	diff2 = RTemp[2] ^ RDiff[2];

	unsigned int T1 = ONX(RTemp[3], RTemp[2], LL(RTemp[1], 10));
	unsigned int T2 = ONX(diff3, diff2, LL(RTemp[1], 10));
	unsigned int T3 = T2 - T1;

	unsigned int count=0,count1=0;
	unsigned int r = 0;
	for (unsigned int i = 0; i < 0x20000000; i++){// verify Y1
		//compute Y1
		RTemp[0] = 0;
		RTemp[0] = i & 0x7;//0-2[3]
		RTemp[0] = RTemp[0] | (((i >> 3) & 0x3ff) << 4);//3-12[10]
		RTemp[0] = RTemp[0] | (((i >> 13) & 0x3fff) << 15);//13-26[14]
		RTemp[0] = RTemp[0] | (((i >> 27) & 0x3) << 30);//27-28[2]
		RTemp[0] = RTemp[0] ^ 0x20004008;

		//word[2] = RR(RTemp[5] - LL(RTemp[1], 10), RS[0][5]) - (ONX(RTemp[4], RTemp[3], LL(RTemp[2], 10)) +LL(RTemp[0],10) + RC[0]);

		r = RR(RTemp[4] - LL(RTemp[0],10), RS[0][4]) + t1;
		if (RR(diff4 - LL(RTemp[0], 10), RS[0][4]) == r){//Have DeltaRIV[4]=0
			RIV[4] = RR(RTemp[4] - LL(RTemp[0], 10), RS[0][4]) - (ONX(RTemp[3], RTemp[2], LL(RTemp[1], 10)) + word[RI[0][4]] + RC[0]);//M9 is konwn

			T1 = ONX(RTemp[2], RTemp[1], LL(RTemp[0], 10));
			T2 = ONX(diff2, RTemp[1], LL(RTemp[0], 10));
			t0 = T2 - T1;
			r = RR(RTemp[3] - RIV[4], RS[0][3]) + t0;
			if (RR(diff3 - RIV[4], RS[0][3]) == r){//Have DeltaRIV[3]=0
				validRT0[count] = RTemp[0];
				count++;
			}
		}
	}
	//cout << hex << count << endl;
	//system("pause");
	cout << hex << "Finished computing valid m2. There are 0x" << count << " valid m2!" << endl;
	return true;
}

//check whether RIV[3](Y-1) is valid
bool RIPEMD160::verifyRIV3(){
	//Under the circumstance that RIV[3] is already known.
	unsigned int diff2 = 0, r,count=0;
	diff2 = RTemp[2] ^ RDiff[2];
	for (int i = 0; i < 0xffffffff; i++){
		r = RR(RTemp[2] - LL(i, 10), 9) + 0x40008010;
		if (RR(diff2 - LL(i, 10), 9) == r){//To ensure DeltaRIV[2]=0
			count++;
			//return true;
		}
	}
	r = RR(RTemp[2] - LL(0xffffffff, 10), 9) + 0x40008010;
	if (RR(diff2 - LL(0xffffffff, 10), 9) == r){//To ensure DeltaRIV[2]=0
		count++;
		//return true;
	}
	cout << hex << count << endl;
	return true;
}

//Verify the result:
void RIPEMD160::verifyResult(){
	word[0] = 0x678c8c36;
	word[1] = 0x67dbd0a9;
	word[2] = 0x5293b823;
	word[3] = 0xd90c1aa9;
	word[4] = 0x5cd30b65;
	word[5] = 0x13d3dff6;
	word[6] = 0x651c397d;
	word[7] = 0x794a60c6;
	word[8] = 0x50ff865;
	word[9] = 0x471fba32;
	word[10] = 0xa9f94c09;
	word[11] = 0x509bf856;
	word[12] = 0x588c327;
	word[13] = 0x86671566;
	word[14] = 0xee8e443e;
	word[15] = 0xc3349b51;

	LIV[0] = 0xd055ce6;
	LIV[1] = 0xdf979ac7;
	LIV[2] = 0xae4836b3;
	LIV[3] = 0x57b6f5fb;
	LIV[4] = 0x6b9ec934;

	RIV[0] = LIV[0];
	RIV[1] = LIV[1];
	RIV[2] = LIV[2];
	RIV[3] = LIV[3];
	RIV[4] = LIV[4];
	computeHash(0, 14);
	word[7] = word[7] + 0x40008010;
	cout << "After inserting difference!" << endl;
	computeHash(0, 14);
	system("pause");
}

//merging
int main(){
	RIPEMD160 ripemd;
	cout << "0-->Verify one result!" << endl;
	cout << "1--> Compute until X_{-3}, Y_{-3}, the probability is about 2^{-0.4}!" << endl;
	cout << "2--> Compute until X_{_4}, Y_{-4} to verify the complexity of the merging phase!" << endl;
	cout << "Please input command:";

	int cmd = 0;
	cin >> cmd;
	if (cmd == 0){
		ripemd.verifyResult();
	}
	if (cmd == 1){
		ripemd.initialize();
		ripemd.merge(false);
	}
	else{
		ripemd.initialize();
		ripemd.merge(true);
	}
	system("pause");
	return 0;
}

//test the probability that M3 makes the differential path held. (Right Branch)
//valid number:
int main1(){
	RIPEMD160 ripemd;
	ripemd.initialize();
	ripemd.verifyM3();
	system("pause");
	return 0;
}

//test the probability that M14 makes the differential path held. (Left Branch)
//valid number: 0x810000
int main2(){
	RIPEMD160 ripemd;
	ripemd.initialize();
	//ripemd.testVerifyingProbabilityLeft();
	ripemd.verifyM14();
	system("pause");
	return 0;
}

//test the influence of X5 and X4 on the difference while computing backward.
int main3(){
	RIPEMD160 ripemd;
	ripemd.initialize();
	ripemd.verifyX5X4();
	system("pause");
	return 0;
}



