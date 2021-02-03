#include <iostream>
#include <vector>
#include <utility>
#include <map>
#include <cmath>
#include <climits>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
using namespace std;

int color;
int depth_limit = 4;
int stratergy = 1;
int steps=0;
int numnodes;
int N,M;
float w[8];
class list{
public:
	vector<int> l;
	map<int,int> index;
    list(){
    }

    list(const list& l2){
    	for(int i=0;i<(l2.l).size();i++){
    		(this->l).push_back((l2.l)[i]);
    		index[(l2.l)[i]]=(this->l).size();
    	}
    }

	void insert(int p){
		if(index[p]==0)
			l.push_back(p);
			index[p] = l.size();
	}

	void del(int p){
		if(index[p]!=0){
			int t = index[p];
			index[p]=0;
			l.erase(l.begin()+t-1);
			for(int i=t-1;i<l.size();i++){
				index[l[i]]=i+1;
			}
		}
	}

    bool has(int p){
    	return !(index[p]==0);
    }

    int size(){
    	return l.size();
    }

    int get(int i){
    	return l[i];
    }

    void display(){
    	for(int i=0;i<l.size();i++){
    		cout<<"("<<l[i]%M<<","<<l[i]/M<<")"<<" ";
    	}
    	cout<<endl;
    }
};

class Cannon_list{
public:
vector< pair<int,int> > Cannons;
map<int,bool> has_map;
Cannon_list(){
}

Cannon_list(list &Soldier,list &oppSoldiers,list &mytownhall,list &opptownhall){
int change;
int arr[8] = {M,M-1,M+1,1,-1,-M-1,-M+1,-M};
for(int j=0;j<8;j++){
change = arr[j];

for(int i=0;i<Soldier.size();i++){
int ini_pos = Soldier.get(i);

int pos = ini_pos+3*change;
if(pos>=0&&pos<N*M&&abs(pos%M-ini_pos%M)<=3){
if(Soldier.has(ini_pos+change)&&Soldier.has(ini_pos+2*change)&&!Soldier.has(pos)&&!oppSoldiers.has(pos)&&!mytownhall.has(pos)&&!opptownhall.has(pos)){
Cannons.push_back(pair<int,int> (ini_pos,pos-change));
has_map[ini_pos]=true;
has_map[pos-change]=true;
has_map[(ini_pos+pos-change)/2]=true;
}

}
}

   }
}

Cannon_list(const Cannon_list* l2){
    for(int i=0;i<(l2->Cannons).size();i++){
    (this->Cannons).push_back((l2->Cannons)[i]);
    }
   }

        pair<int,int> get(int i){
        return Cannons[i];
        }

   bool has_Soldier(pair<int,int> p,int s){
    if((p.first-p.second)%M==0)
    return ((p.first-s)*(s-p.second))>=0&&(s-p.first)%M;
    else if((p.first-p.second)%(M+1)==0)
    return ((p.first-s)*(s-p.second))>=0&&(s-p.first)%(M+1);
            else if((p.first-p.second)%(M-1)==0)
    return ((p.first-s)*(s-p.second))>=0&&(s-p.first)%(M-1);
  else
  return ((p.first-s)*(s-p.second))>=0;
   }

        bool has(int p){
        	return has_map[p];
        }

   int size(){
    return Cannons.size();
   }

        void display(){
    for(int i=0;i<Cannons.size();i++){
    cout<<"("<<Cannons[i].first<<","<<Cannons[i].second<<")"<<" ";
    }
    cout<<endl;
   }
};

class Move{
public:
	bool bomb;
	int init_pos;
	int final_pos;
	int color;
    Move(int color,int init,int final,bool bomb){
    	this->color = color;
    	init_pos = init;
    	final_pos = final;
		this->bomb = bomb;
    }
    void print(){
    	if(!bomb)
    		cout<<"S "<<init_pos%M<<" "<<init_pos/M<<" M "<<final_pos%M<<" "<<final_pos/M<<endl;
    	else
    		cout<<"S "<<init_pos%M<<" "<<init_pos/M<<" B "<<final_pos%M<<" "<<final_pos/M<<endl;
    }
		void print1(){
			if(!bomb)
				cerr <<"S "<<init_pos%M<<" "<<init_pos/M<<" M "<<final_pos%M<<" "<<final_pos/M<<endl;
			else
				cerr <<"S "<<init_pos%M<<" "<<init_pos/M<<" B "<<final_pos%M<<" "<<final_pos/M<<endl;
		}
};

class State{
public:
	list black_S;
	list white_S;
	list black_T;
	list white_T;
	Cannon_list Cannon_black;
	Cannon_list Cannon_white;
	int skipmoves;
	vector< pair<float,Move> > moves;
	State(list &black_S,list &white_S,list &black_T,list &white_T){
		this->black_S = black_S;
		this->white_S = white_S;
		this->black_T = black_T;
		this->white_T = white_T;
		this->Cannon_black = new Cannon_list(black_S,white_S,black_T,white_T);
		this->Cannon_white = new Cannon_list(white_S,black_S,white_T,black_T);
		skipmoves = 0;
	}
	void update_cannon(){
		this->Cannon_black = new Cannon_list(black_S,white_S,black_T,white_T);
		this->Cannon_white = new Cannon_list(white_S,black_S,white_T,black_T);
	}
	float max(float a,float b){
   		if(a>b){
   			return a;
   		}
   		else{
   			return b;
   		}
   	}
   	float min(float a,float b){
   		if(a>b){
   			return b;
   		}
   		else{
   			return a;
   		}
   	}
	State make_move(Move mv){
		if(!mv.bomb){
			if(mv.color==1)
				{
					white_S.del(mv.init_pos);
				    white_S.insert(mv.final_pos);
				    if(black_S.has(mv.final_pos))
						black_S.del(mv.final_pos);
					else if(black_T.has(mv.final_pos))
						black_T.del(mv.final_pos);
				}
			else
				{
					black_S.del(mv.init_pos);
				    black_S.insert(mv.final_pos);
					if(white_S.has(mv.final_pos))
						white_S.del(mv.final_pos);
					else if(white_T.has(mv.final_pos))
						white_T.del(mv.final_pos);
				}
		}

		else{
			if(mv.color==1){
					if(black_S.has(mv.final_pos))
						black_S.del(mv.final_pos);
					else if(black_T.has(mv.final_pos))
						black_T.del(mv.final_pos);
			}
			else{
					if(white_S.has(mv.final_pos))
						white_S.del(mv.final_pos);
					else if(white_T.has(mv.final_pos))
						white_T.del(mv.final_pos);
			}
		}
		this->update_cannon();
		return State(black_S,white_S,black_T,white_T);

	}
	void unmake_move(Move mv,int killed){
		if(!mv.bomb){
			if(mv.color==1){
					white_S.del(mv.final_pos);
				    white_S.insert(mv.init_pos);
			}
			else{
					black_S.del(mv.final_pos);
				    black_S.insert(mv.init_pos);
			}
		}

		if(killed==-1)
			black_S.insert(mv.final_pos);
		else if(killed==1)
			white_S.insert(mv.final_pos);
		else if(killed==2)
			white_T.insert(mv.final_pos);
		else if(killed==-2)
			black_T.insert(mv.final_pos);
		this->update_cannon();
	}
    void possible_moves(int turn){
    	moves.clear();
    	if(turn==-1){

    		for(int i=0;i<Cannon_black.size();i++){
				pair<int,int> Cann_pair = Cannon_black.get(i);
				int move_pos = (3*Cann_pair.second-Cann_pair.first)/2;
						moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,move_pos,false)));
						int shoot_pos = (2*Cann_pair.second-Cann_pair.first);
					if(valid_move(Move (turn,Cann_pair.first,shoot_pos,true))){
						if(white_S.has(shoot_pos)||white_T.has(shoot_pos))
						moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    else if(skipmoves==0){
					    	moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    	skipmoves++;
					    }
					}
					shoot_pos = (5*Cann_pair.second-3*Cann_pair.first)/2;
					if(valid_move(Move (turn,Cann_pair.first,shoot_pos,true))){
						if(white_S.has(shoot_pos)||white_T.has(shoot_pos)){
							moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    }
					    else if(skipmoves==0){
					    	moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    	skipmoves++;
					    }

					}
			}

			for(int i=0;i<black_S.size();i++){
				int position = black_S.get(i);
				if(position/M!=0){
					if(valid_move(Move (turn,position,position-M,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position-M,false)));
					if(position%M!=(M-1)&&valid_move(Move (turn,position,position-(M-1),false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position-(M-1),false)));
					if(position%M!=0&&valid_move(Move (turn,position,position-(M+1),false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position-(M+1),false)));
				}
				if(position%M!=0&&valid_move(Move (turn,position,position-1,false)))
					moves.push_back(pair<float,Move> (0,Move (turn,position,position-1,false)));
				if(position%M!=(M-1)&&valid_move(Move (turn,position,position+1,false)))
					moves.push_back(pair<float,Move> (0,Move (turn,position,position+1,false)));
				if(position/M<(N-2)){
					if(valid_move(Move (turn,position,position+2*M,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position+2*M,false)));
					if(position%M<(M-2)&&valid_move(Move (turn,position,position+2*M+2,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position+2*M+2,false)));
					if(position%M>1&&valid_move(Move (turn,position,position+2*M-2,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position+2*M-2,false)));
				}
			}


		}
		else{

			for(int i=0;i<Cannon_white.size();i++){
				pair<int,int> Cann_pair = Cannon_white.get(i);
					int move_pos = (3*Cann_pair.second-Cann_pair.first)/2;
					moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,move_pos,false)));

					int shoot_pos = (2*Cann_pair.second-Cann_pair.first);
					if(valid_move(Move (turn,Cann_pair.first,shoot_pos,true))){
						if(black_S.has(shoot_pos)||black_T.has(shoot_pos)){
							moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    }
					    else if(skipmoves==0){
					    	moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    	skipmoves++;
					    }

					}

					shoot_pos = (5*Cann_pair.second-3*Cann_pair.first)/2;
					if(valid_move(Move (turn,Cann_pair.first,shoot_pos,true))){
						if(black_S.has(shoot_pos)||black_T.has(shoot_pos)){
							moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    }
					    else if(skipmoves==0){
					    	moves.push_back(pair<float,Move> (0,Move (turn,Cann_pair.first,shoot_pos,true)));
					    	skipmoves++;
					    }

					}

			}

			for(int i=0;i<white_S.size();i++){
				int position = white_S.get(i);
				if(position/M!=(N-1)){
					if(valid_move(Move (turn,position,position+M,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position+M,false)));
					if(position%M!=0&&valid_move(Move (turn,position,position+(M-1),false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position+(M-1),false)));
					if(position%M!=(M-1)&&valid_move(Move (turn,position,position+(M+1),false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position+(M+1),false)));
				}
				if(position%M!=(M-1)&&valid_move(Move (turn,position,position+1,false)))
					moves.push_back(pair<float,Move> (0,Move (turn,position,position+1,false)));
				if(position%M!=0&&valid_move(Move (turn,position,position-1,false)))
					moves.push_back(pair<float,Move> (0,Move (turn,position,position-1,false)));
				if(position/M>1){
					if(valid_move(Move (turn,position,position-2*M,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position-2*M,false)));
					if(position%M>1&&valid_move(Move (turn,position,position-2*M-2,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position-2*M-2,false)));
					if(position%M<(M-2)&&valid_move(Move (turn,position,position-2*M+2,false)))
						moves.push_back(pair<float,Move> (0,Move (turn,position,position-2*M+2,false)));
				}
			}
		}
    }

	bool valid_move(Move mv){
		if(mv.init_pos<0||mv.final_pos<0||mv.init_pos>=N*M||mv.final_pos>=N*M)
			return false;
		if(mv.color==-1){
			if(mv.bomb){

				 if(!black_S.has(mv.final_pos)&&!black_T.has(mv.final_pos))
				 {
				 	int change;
				 	if((mv.final_pos-mv.init_pos)%5==0)
		       		 	change = (mv.final_pos-mv.init_pos)/5;
		       		else
		       			change = (mv.final_pos-mv.init_pos)/4;
				    return abs((mv.init_pos+3*change)%M-mv.final_pos%M)<=2;
				 }
			}
			else
			{
				if(black_S.has(mv.final_pos)||black_T.has(mv.final_pos))
					return false;
				if((mv.final_pos==mv.init_pos-M||mv.final_pos==mv.init_pos-(M-1)||mv.final_pos==mv.init_pos-(M+1)))
					return true;
				if((mv.final_pos==mv.init_pos+1||mv.final_pos==mv.init_pos-1))
					return white_S.has(mv.final_pos)||white_T.has(mv.final_pos);
				if((mv.final_pos==mv.init_pos+2*M||mv.final_pos==mv.init_pos+2*M-2||mv.final_pos==mv.init_pos+2*M+2)){
					bool right = mv.init_pos%M!=(M-1)&&white_S.has(mv.init_pos+1);
					bool left = mv.init_pos%M!=0&&white_S.has(mv.init_pos-1);
					bool top = white_S.has(mv.init_pos-M);
					bool topright = mv.init_pos%M!=(M-1)&&white_S.has(mv.init_pos-(M-1));
					bool topleft = mv.init_pos%M!=0&&white_S.has(mv.init_pos-(M+1));
				    return right||left||top||topright||topleft;
				}
			}
		}
		else{
			if(mv.bomb){
				if(!white_S.has(mv.final_pos)&&!white_T.has(mv.final_pos))
				 {
				 	int change;
				 	if((mv.final_pos-mv.init_pos)%5==0)
		       		 	change = (mv.final_pos-mv.init_pos)/5;
		       		else
		       			change = (mv.final_pos-mv.init_pos)/4;
				    return abs((mv.init_pos+3*change)%M-mv.final_pos%M)<=2;
				 }
			}
			else
			{
				if(white_S.has(mv.final_pos)||white_T.has(mv.final_pos))
					return false;
				if((mv.final_pos==mv.init_pos+M||mv.final_pos==mv.init_pos+(M-1)||mv.final_pos==mv.init_pos+(M+1)))
					return true;
				if((mv.final_pos==mv.init_pos+1||mv.final_pos==mv.init_pos-1))
					return black_S.has(mv.final_pos)||black_T.has(mv.final_pos);
				if((mv.final_pos==mv.init_pos-2*M||mv.final_pos==mv.init_pos-2*M+2||mv.final_pos==mv.init_pos-2*M-2)){
					bool right = mv.init_pos%M!=0&&black_S.has(mv.init_pos-1);
					bool left = mv.init_pos%M!=(M-1)&&black_S.has(mv.init_pos+1);
					bool top = black_S.has(mv.init_pos+M);
					bool topright = mv.init_pos%M!=0&&black_S.has(mv.init_pos+(M-1));
					bool topleft = mv.init_pos%M!=(M-1)&&black_S.has(mv.init_pos+(M+1));
				    return right||left||top||topright||topleft;
				}
			}
		}
	return false;
	}

    string display(){
		string q="";
		for(int i=0;i<8;i++){
			for(int j=0;j<8;j++){
				if(black_S.has(i*8+j)){
					q+='B';
					// cerr<<"B ";
				}
				else if(white_S.has(i*8+j)){
					q+='W';
					// cerr<<"W ";
				}
				else if(white_T.has(i*8+j)){
					q+='O';
					// cerr<<"O ";
				}
				else if(black_T.has(i*8+j)){
					q+='T';
					// cerr<<"T ";
				}
			    else{
			    	q+='_';
			    	// cerr<<"_ ";
			    }
			}
			// cerr<<endl;
		}
		return q;
	}
		int man_dis(int a,int b){
        return abs(a%M-b%M)*abs(a%M-b%M)+abs(a/M-b/M)*abs(a/M-b/M);
    }
		float man_dis1(int a, int b){
				int x = abs(a%M-b%M);
				int y = abs(a/M-b/M);
				if(x>2){
					return 10;
				}
				return sqrt(x*x*x) + sqrt(y*y*y);
		}
    float hit_from_Cannon_Black(float a,float b,float c){
    	// if(N==8 && M==8){
				   float utility1=0;
				float util =0;
	    	for(int i=0;i<Cannon_black.size();i++){
	    		pair<int,int> Cann_pair = Cannon_black.get(i);
					utility1 = 0;
					if(abs(Cann_pair.first-Cann_pair.second)<=3){
						utility1 += a;
					}
					else{
						utility1 += b;
					}
	    		int pos0=(3*Cann_pair.second-Cann_pair.first)/2;
	    		int pos1=(2*Cann_pair.second-Cann_pair.first);
	    		int pos2=(5*Cann_pair.second-3*Cann_pair.first)/2;

	    		if(abs(pos0%M-pos1%M)<=1&&pos1>=0&&pos1<N*M){
	    			if(white_S.has(pos1))
	    				utility1 += 1;
						if(Cannon_white.has(pos1)){
							utility1 += 1;
						}
	    			if(white_T.has(pos1))
	    				utility1 += 5;
	    		}

	    		if(abs(pos0%M-pos2%M)<=2&&pos2>=0&&pos2<N*M){
	    			if(white_S.has(pos2))
	    				utility1 += 1;
						if(Cannon_white.has(pos2)){
							utility1 += 1;
						}
	    			if(white_T.has(pos2))
	    				utility1 += 5;
	    		}
					util += min(7,utility1);
	    	}
	    	return util;
	    // }
	    // else if(N==10 && M==8){
			//
	    // }
	    // else{
			//
	    // }
	    // return 0;
    }
		float hit_from_Cannon_White(float a,float b,float c){
			// if(N==8 && M==8){
			   float utility1=0;
				float util =0;
				for(int i=0;i<Cannon_white.size();i++){
					pair<int,int> Cann_pair = Cannon_white.get(i);
					utility1 = 0;
					if(abs(Cann_pair.first-Cann_pair.second)==3){
						utility1 += a;
					}
					else{
						utility1 += b;
					}
					int pos0=(3*Cann_pair.second-Cann_pair.first)/2;
					int pos1=(2*Cann_pair.second-Cann_pair.first);
					int pos2=(5*Cann_pair.second-3*Cann_pair.first)/2;

					if(abs(pos0%M-pos1%M)<=1&&pos1>=0&&pos1<N*M){
						if(black_S.has(pos1))
							utility1 += 1;
						if(Cannon_black.has(pos1)){
							utility1 += 1;
						}
						if(black_T.has(pos1))
							utility1 += 5;
					}

					if(abs(pos0%M-pos2%M)<=2&&pos2>=0&&pos2<N*M){
						if(black_S.has(pos2))
							utility1 += 1;
						if(Cannon_black.has(pos2)){
							utility1 += 1;
						}
						if(black_T.has(pos2))
							utility1 += 5;
					}
					util += min(7,utility1);
				}
				return util;
			// }
			// else if(N==10 && M==8){
			//
			// }
			// else{
			//
			// }
			// return 0;
		}
    int white_town_attack(){
    	// if(N==8 && M==8){
	        int utility1=0;
	        vector<int> town_hall_attack(white_T.size(),0);
	        for(int j=0;j<white_T.size();j++){
	            for(int i=0;i<black_S.size();i++){
	                town_hall_attack[j] += max(0,10-man_dis(black_S.get(i),white_T.get(j)));
	            }
	            utility1 += min(15,town_hall_attack[j]);

	        }
	        return utility1;
	    // }
	    // else if(N==10 && M==8){
			//
	    // }
	    // else{
			//
	    // }
	    // return 0;
    }
    int black_town_attack(){
    	// if(N==8 && M==10){
	        int utility1=0;
	        vector<int> town_hall_attack(black_T.size(),0);
	        for(int j=0;j<black_T.size();j++){
	            for(int i=0;i<white_S.size();i++){
	                town_hall_attack[j] += max(0,10-man_dis(white_S.get(i),black_T.get(j)));
	            }
	            utility1 += min(15,town_hall_attack[j]);

	        }
	        utility1 = utility1;
	        return utility1;
    	// }
    	// else if(N==10 && M==8){
			//
    	// }
    	// else{
			//
    	// }
	    // return 0;


    }
    int black_town_safety(){
    	// if(N==8 && M==8){
	        int utility1=0;
	        vector<int> town_hall_safety(black_T.size(),0);
	        for(int j=0;j<black_T.size();j++){
	            for(int i=0;i<black_S.size();i++){
	                town_hall_safety[j] +=  max(0,10-man_dis(black_S.get(i),black_T.get(j)));
	            }
	            utility1 += min(15,town_hall_safety[j]);
	        }
	        utility1 = utility1;
	        return utility1;
    	// }
    	// else if(N==10 && M==8){
			//
    	// }
    	// else{
			//
    	// }
	    // return 0;


    }
    int white_town_safety(){
    	// if(N==8 && M==8){
	        int utility1=0;
	        vector<int> town_hall_safety(white_T.size(),0);
	        for(int j=0;j<white_T.size();j++){
	            for(int i=0;i<white_S.size();i++){
	                town_hall_safety[j] +=  max(0,3-man_dis(white_S.get(i),white_T.get(j)));
	            }
	            utility1 += min(15,town_hall_safety[j]);
	        }
	        utility1 = utility1;
	        return utility1;
    	// }
    	// else if(N==10 && M==8){
			//
    	// }
    	// else{
			//
    	// }


    }
    int white_town_hall_attack_defence_utility(){
    	if(N==8){
    		int utility1=0;
	    	vector<int> town_hall_white(white_T.size(),0);
	    	for(int j=0;j<white_T.size();j++){
	    		for(int i=0;i<black_S.size();i++){
	    			town_hall_white[j] += max(0,10-man_dis(black_S.get(i),white_T.get(j)));
	    		}
	    		int attack_factor = town_hall_white[j];
	    		town_hall_white[j] = min(15,town_hall_white[j]);
	    		int safe=0;
	    		for(int i=0;i<white_S.size();i++){
	    			safe+=max(0,10-man_dis(white_S.get(i),white_T.get(j)));
	    		}
	    		int defence_factor = town_hall_white[j];
	    		safe = min(15,safe);
	    		utility1 += max(town_hall_white[j]-safe,-2);

	    	}
	    	return utility1;
    	}
    	else{
				int utility1=0;
	    	vector<int> town_hall_white(white_T.size(),0);
	    	for(int j=0;j<white_T.size();j++){
	    		for(int i=0;i<black_S.size();i++){
	    			town_hall_white[j] += max(0,10-man_dis1(black_S.get(i),white_T.get(j)));
	    		}
	    		int attack_factor = town_hall_white[j];
	    		town_hall_white[j] = min(15,town_hall_white[j]);
	    		int safe=0;
	    		for(int i=0;i<white_S.size();i++){
	    			safe+=max(0,10-man_dis1(white_S.get(i),white_T.get(j)));
	    		}
	    		int defence_factor = town_hall_white[j];
	    		safe = min(15,safe);
	    		utility1 += max(town_hall_white[j]-safe,-3);

	    	}
	    	return utility1;
    	}
	    return 0;

    }
    int black_town_hall_attack_defence_utility(){
    	if(N==8 && M==8){
    		int utility1=0;
	    	vector<int> town_hall_black(black_T.size(),0);
	    	for(int j=0;j<black_T.size();j++){
	    		for(int i=0;i<white_S.size();i++){
	    			town_hall_black[j] += max(0,10-man_dis(white_S.get(i),black_T.get(j)));
	    		}
	    		int attack_factor = town_hall_black[j];
	    		town_hall_black[j] = min(15,town_hall_black[j]);
	    		int safe=0;
	    		for(int i=0;i<black_S.size();i++){
	    			safe+=max(0,10-man_dis(black_S.get(i),black_T.get(j)));
	    		}
	    		int defence_factor = town_hall_black[j];
	    		safe = min(15,safe);
	    		utility1 += max(town_hall_black[j]-safe,-2);
	    	}
	    	return utility1;
    	}
    	else{
				int utility1=0;
				vector<int> town_hall_black(black_T.size(),0);
				for(int j=0;j<black_T.size();j++){
					for(int i=0;i<white_S.size();i++){
						town_hall_black[j] += max(0,10-man_dis1(white_S.get(i),black_T.get(j)));
					}
					int attack_factor = town_hall_black[j];
					town_hall_black[j] = min(15,town_hall_black[j]);
					int safe=0;
					for(int i=0;i<black_S.size();i++){
						safe+=max(0,10-man_dis1(black_S.get(i),black_T.get(j)));
					}
					int defence_factor = town_hall_black[j];
					safe = min(15,safe);
					utility1 += max(town_hall_black[j]-safe,-3);
				}
				return utility1;
    	}
	    return 0;


    }
    float utility(){
		    	int mod;
				if(black_S.size()==white_S.size() )
					mod=0;
				else
					mod = abs(black_S.size()-white_S.size())/(black_S.size()-white_S.size());
				float utility1 =0;

		    	if(stratergy ==1){
			    		utility1 += (w[0]-w[2]*color)*black_S.size()-(w[0]+w[2]*color)*white_S.size();
			    		utility1 += (w[4])*(hit_from_Cannon_Black(0.5,1.5,1.5))-(w[4])*(hit_from_Cannon_White(0.5,1.5,1.5));
			    		utility1 += w[1]*(black_T.size()-white_T.size());
							// cerr <<utility1 << " " << (w[0]-w[2]*color)*black_S.size()-(w[0]+w[2]*color)*white_S.size() <<" " << (w[4])*(hit_from_Cannon_Black(0.5,1.5,1.5))-(w[4])*(hit_from_Cannon_White(0.5,1.5,1.5)) <<" " <<  w[1]*(black_T.size()-white_T.size()) << endl;
		    	}
		    	else if(stratergy==2){
			        utility1 += (w[0]-w[2]*color)*black_S.size()-(w[0]+w[2]*color)*white_S.size();
			        utility1 += w[4]*(hit_from_Cannon_Black(0.5,2,2)-hit_from_Cannon_White(0.5,2,2));
							utility1 += (w[7]-2*color)*(white_town_hall_attack_defence_utility())-(w[7]+2*color)*(black_town_hall_attack_defence_utility());
			        utility1 += w[1]*(black_T.size()-white_T.size());
					}
					else if(stratergy==3){
								utility1 += (w[0]-w[2]*mod)*black_S.size()-(w[0]+w[2]*mod)*white_S.size();
								utility1 += w[4]*(hit_from_Cannon_Black(0.5,2,2)-hit_from_Cannon_White(0.5,2,2));
								utility1 += (w[7]+2*mod)*(white_town_hall_attack_defence_utility())-(w[7]-2*mod)*(black_town_hall_attack_defence_utility());
								utility1 += w[1]*(black_T.size()-white_T.size());
					}
					else if(stratergy ==4){
								utility1 += (w[0]-w[2]*color)*black_S.size() -(w[0]+w[2]*color)*white_S.size();
				        utility1 += (w[4])*(hit_from_Cannon_Black(1,2,2)-hit_from_Cannon_White(1,2,2));
				        utility1 += (w[7]-2*color)*(white_town_hall_attack_defence_utility())-(w[7]+2*color)*(black_town_hall_attack_defence_utility());
				       	utility1 += w[1]*(black_T.size()-white_T.size());
					}
					else if(stratergy ==5){
								utility1 += (w[0]-w[2]*mod)*black_S.size() -(w[0]+w[2]*mod)*white_S.size();
				        utility1 += ((w[4])+mod)*(hit_from_Cannon_Black(1.5,1.5,1.5))-((w[4])-mod)*(hit_from_Cannon_White(1.5,1.5,1.5));
				        utility1 += (w[7]+2*mod)*white_town_hall_attack_defence_utility()-(w[7]-2*mod)*black_town_hall_attack_defence_utility();
				       	utility1 += w[1]*(black_T.size()-white_T.size());
					}
					else if(stratergy ==6){
								utility1 += ((w[0])-w[2]*color)*black_S.size() -((w[0])+w[2]*color)*white_S.size();
				        utility1 += ((w[4])-color)*hit_from_Cannon_Black(1.5,1.5,1.5)-((w[4])+color)*(hit_from_Cannon_White(1.5,1.5,1.5));
				        utility1 += (w[7]-color)*white_town_hall_attack_defence_utility()-(w[7]+color)*black_town_hall_attack_defence_utility();
				       	utility1 += w[1]*(black_T.size()-white_T.size());
					}
					else if(stratergy==7){
								utility1 += (w[0]-w[2]*color)*black_S.size()-(w[0]+w[2]*color)*white_S.size();
				        utility1 += ((w[4])+mod)*(hit_from_Cannon_Black(2,1.5,1.5))-((w[4])-mod)*(hit_from_Cannon_White(2,1.5,1.5));
				        utility1 += (w[7]+2*mod)*white_town_hall_attack_defence_utility()-(w[7]-2*mod)*black_town_hall_attack_defence_utility();
				       	utility1 += w[1]*(black_T.size()-white_T.size());
					}
					else{
								utility1 += (w[0]-w[2]*color)*black_S.size()-(w[0]+w[2]*color)*white_S.size();
				        utility1 += w[4]*(hit_from_Cannon_Black(0,0,0)-hit_from_Cannon_White(0,0,0));
				        utility1 += ((w[7])-color)*white_town_hall_attack_defence_utility()-((w[7])+color)*black_town_hall_attack_defence_utility();
				       	utility1 += w[1]*(black_T.size()-white_T.size());
					}
				if(color==-1)
			        return utility1;
        else
        	return -utility1;
		    }
};

bool compare(pair<float,Move> &i1,pair<float,Move> &i2)
{
    return (i1.first > i2.first);
}
void assign_weight(char *s){
	ifstream myfile(s);
	string line,temp;
	int i=0;
	if(myfile.is_open()){
		while(!myfile.eof()){
			stringstream ss;
            getline (myfile,line);
            ss.str(line);
            ss>>temp;
            w[i] = stof(temp);
            i++;
		}
		return;
		myfile.close();

	}
}
map<string,int> existed;
list iblack_S;
list iwhite_S;
list iblack_T;
list iwhite_T;
State curr_state(iblack_S,iwhite_S,iblack_T,iwhite_T);
// pair<float,Move> nextMove(State st,int depth,Move mv,float alpha,float beta,int limit){
//     	numnodes++;
//    		if(mv.init_pos!=mv.final_pos&&(depth==limit||st.white_T.size()==(M/2-2)||st.black_T.size()==(M/2-2)||st.black_S.size()==0||st.white_S.size()==0)){
//     		float t= st.utility();
//     		return pair<float,Move> (t,mv);
//     	}
//     	pair<float,Move> Bestmov = pair<float,Move> (INT_MIN,Move(1,1,0,false));
// 			if(depth==0&&curr_state.moves.size()!=0){
// 				st.moves = curr_state.moves;
// 				cerr<<st.moves.size()<<endl;
// 			}
// 			if(st.moves.size()==0){
// 				if(depth%2==1)
// 	    		{
// 			  		Bestmov = pair<float,Move> (INT_MAX,Move (0,0,0,false));
// 	              	st.possible_moves(-color);
// 	            }
// 	    	else
// 	    		{
// 	    			Bestmov = pair<float,Move> (INT_MIN,Move (0,0,0,false));
// 	    			st.possible_moves(color);
// 	    		}
// 			}
//     		if(mv.init_pos!=mv.final_pos&&st.moves.size()==0)
//     			return pair<float,Move> (st.utility(),mv);
//     	if(depth==0)
//     		curr_state.moves.clear();
//     	for(int i=0;i<(st.moves).size();i++){
//     		if(depth==0){
//     			curr_state.moves.push_back(st.moves[i]);
//     		}
//     		int kill = 0;
//     		if((st.black_S).has(((st.moves)[i].second).final_pos))
//     			kill = -1;
//     		else if((st.white_S).has(((st.moves)[i].second).final_pos))
//     			kill = 1;
//     		else if((st.white_T).has(((st.moves)[i].second).final_pos))
//     			kill = 2;
//     		else if((st.black_T).has(((st.moves)[i].second).final_pos))
//     			kill = -2;
//     		State ts = (st.make_move((st.moves)[i].second));
//     		string sf = ts.display();
//     		if(existed.find(sf)==existed.end()||existed[sf]<1){
// 	    		pair<float,Move> s = nextMove(ts,depth+1,(st.moves)[i].second,alpha,beta,limit);
// 	    		if(depth==0){
// 	    		 curr_state.moves[i].first = s.first;
// 	    		 (st.moves)[i].first = s.first;
// 	    		}
//
// 	    		if(s.first>Bestmov.first&&depth%2==0){
// 	    			alpha = max(alpha,s.first);
// 	    			Bestmov = pair<float,Move>(s.first,(st.moves)[i].second);
// 	    		}
// 	    		else if(s.first<Bestmov.first&&depth%2==1){
// 	    			beta = min(beta,s.first);
// 	    			Bestmov = pair<float,Move>(s.first,(st.moves)[i].second);
// 	    		}
//
// 	    		if(beta<=alpha)
// 	    			break;
// 	    	}
//
//     		st.unmake_move((st.moves)[i].second,kill);
//     	}
//     	return Bestmov;
//     }
pair<float,Move> nextMove(State st,int depth,Move mv,float alpha,float beta,int limit){
		    numnodes++;
		    if(mv.init_pos!=mv.final_pos&&(depth==limit||st.white_T.size()==(M/2-2)||st.black_T.size()==(M/2-2)||st.black_S.size()==0||st.white_S.size()==0)){
		    float t= st.utility();
		    return pair<float,Move> (t,mv);
		    }
		    pair<float,Move> Bestmov = pair<float,Move> (INT_MIN,Move(1,1,0,false));
		    if(depth%2==1)
		  Bestmov = pair<float,Move> (INT_MAX,Move (0,0,0,false));
		    else
		    Bestmov = pair<float,Move> (INT_MIN,Move (0,0,0,false));
		if(depth==0&&curr_state.moves.size()!=0){
		st.moves = curr_state.moves;
		cerr<<st.moves.size()<<endl;
		}
		if(st.moves.size()==0){
		if(depth%2==1)
		    {
		  Bestmov = pair<float,Move> (INT_MAX,Move (0,0,0,false));
		              st.possible_moves(-color);
		           }
		    else
		    {
		    Bestmov = pair<float,Move> (INT_MIN,Move (0,0,0,false));
		    st.possible_moves(color);
		    }
		}
		    if(mv.init_pos!=mv.final_pos&&st.moves.size()==0)
		    return pair<float,Move> (st.utility(),mv);
		    if(depth==0)
		    curr_state.moves.clear();
		    for(int i=0;i<(st.moves).size();i++){
		    if(depth==0){
		    curr_state.moves.push_back(st.moves[i]);
		    }
		    int kill = 0;
		    if((st.black_S).has(((st.moves)[i].second).final_pos))
		    kill = -1;
		    else if((st.white_S).has(((st.moves)[i].second).final_pos))
		    kill = 1;
		    else if((st.white_T).has(((st.moves)[i].second).final_pos))
		    kill = 2;
		    else if((st.black_T).has(((st.moves)[i].second).final_pos))
		    kill = -2;
		    State ts = (st.make_move((st.moves)[i].second));
		    string sf = ts.display();
		    if(existed.find(sf)==existed.end()||existed[sf]<1){
		    pair<float,Move> s = nextMove(ts,depth+1,(st.moves)[i].second,alpha,beta,limit);
		    if(depth==0){
		    curr_state.moves[i].first = s.first;
		    (st.moves)[i].first = s.first;
		    }

		    if(s.first>Bestmov.first&&depth%2==0){
		    alpha = max(alpha,s.first);
		    Bestmov = pair<float,Move>(s.first,(st.moves)[i].second);
		    }
		    else if(s.first<Bestmov.first&&depth%2==1){
		    beta = min(beta,s.first);
		    Bestmov = pair<float,Move>(s.first,(st.moves)[i].second);
		    }

		    if(beta<=alpha)
		    break;
		    }

		    st.unmake_move((st.moves)[i].second,kill);
		    }
		    return Bestmov;
		    }
void update_weights(float w0,float w1,float w2,float w3,float w4,float w5,float w6,float w7){
	w[0] = w0;
	w[1] = w1;
	w[2] = w2;
	w[3] = w3;
	w[4] = w4;
	w[5] = w5;
	w[6] = w6;
	w[7] = w7;
}
int main(int argc, char **argv){
	ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    // assign_weight("weights.txt");
    for(int i=0;i<6;i++){
    	cerr << w[i] << endl;
    }
    int mycolor,t;
    cin>>mycolor>>N>>M>>t;
    for(int i=0;i<M/2;i++){
		curr_state.black_S.insert((N-1)*M+2*i);
		curr_state.black_S.insert((N-2)*M+2*i);
		curr_state.black_S.insert((N-3)*M+2*i);
		curr_state.white_S.insert(2*i+1);
		curr_state.white_S.insert(M+2*i+1);
		curr_state.white_S.insert(2*M+2*i+1);
		curr_state.white_T.insert(2*i);
		curr_state.black_T.insert((N-1)*M+2*i+1);
    }
    curr_state.update_cannon();
    if(mycolor==1)
    	color = -1;
    else
    	color = 1;


	int turn = -1;
	int steps=0;
	int i=0;
	int skipmoves =0;
	float total_time ;
	if(N==8 && M==8){
		total_time = 88;
	}
	else if(N==8 && M==10){
		total_time = 103;
	}
	else if(N==10 && M==10){
		total_time = 118;
	}
	float inital_time = clock();
	float last_move_time;
	float duration_till_now;
	float last_move;
    while(true){
	       	char obj,type;
	       	int x1,y1,x2,y2;
	       	Move mv = Move(0,0,0,false);
        	if(turn==color)
        	{
						inital_time = clock();
						int total_sol = curr_state.black_S.size()+curr_state.white_S.size();
						int diff_sol = abs(curr_state.black_S.size()-curr_state.white_S.size());
	      		numnodes=0;
						if(total_sol<6){
							depth_limit =10;
						}
						if(total_sol<10){
							depth_limit = 6;
						}
	      		if(steps<M-2){
	        			stratergy = 1;
								depth_limit = 4;
								update_weights(40,2000,5,6,5,4,4,6);
						}
	      		else if(total_sol>=(9*M)/4&&diff_sol<3){
	        			stratergy = 2;
								depth_limit = 4;
								update_weights(40,2000,5,6,10,0.25,4,5);
	      		}
	      		else if(total_sol>=(9*M)/4&&diff_sol>=3){
								update_weights(45,2000,10,6,10,0.5,4,6);
								depth_limit = 4;
	        			stratergy = 3;
	      		}
	      		else if(total_sol>=(3*M/2)&&diff_sol<3){
								update_weights(50,2000,5,8,10,4,4,6);
	        			stratergy = 4;
	      		}
	      		else if(total_sol>=(3*M/2)&&diff_sol>=3){
								update_weights(50,2000,10,6,12,0.5,4,6);
	        			stratergy = 5;
	      		}
	      		else if(total_sol>=M&&diff_sol<=2){
	      				stratergy = 6;
								update_weights(55,2000,5,8,10,4,4,7);
	      		}
	      		else if(total_sol>=M&&diff_sol>=2){
	      		  	stratergy = 7;
								update_weights(65,2000,10,8,10,4,4,7);
	      		}
	      		else{
		      			stratergy = 8;
								update_weights(70,2000,5,8,10,4,4,8);
		      			depth_limit = 6;
	      		}

						if(total_time - duration_till_now < 20){
							depth_limit = 4;
						}
						else if(18+last_move_time>total_time-duration_till_now){
							depth_limit = 4;
						}
						if(total_time - duration_till_now < 7){
							depth_limit = 2;
						}
						// depth_limit = 1;
	      		for(int i = 2;i<depth_limit;i=i+2){
	      			Move m1 = nextMove(curr_state,0,Move(0,0,0,false),INT_MIN,INT_MAX,i).second;
	      			cerr<<curr_state.moves.size()<<endl;
	      			sort(curr_state.moves.begin(),curr_state.moves.end(),compare);
	      		}
						// if(N==10 && M ==10)
						// 	curr_state.moves.erase(curr_state.moves.end()-(curr_state.moves.size()/2),curr_state.moves.end());
						cerr  << stratergy << " no of steps " << steps << endl;
						cerr << w[0] << " " << w[1] << " " << w[2] << " " << w[3] << " " << w[4] << " " << w[5] << " " <<  w[6] <<" " << w[7];
	      		Move mv2 = nextMove(curr_state,0,Move(0,0,0,false),INT_MIN,INT_MAX,depth_limit).second;
						sort(curr_state.moves.begin(),curr_state.moves.end(),compare);
						// for(int i=0;i<curr_state.moves.size();i++){
						// 	cerr << curr_state.moves[i].first << " ";
						// 	curr_state.moves[i].second.print1();
						// }
	      	 	mv = mv2;
	      	 	steps++;
						last_move_time = ((float)clock()-inital_time)/CLOCKS_PER_SEC;
						duration_till_now += last_move_time;
						cerr << "last move time : " << last_move_time << " duration till now " << duration_till_now << endl;
						cerr << "No of nodes opened till now " << numnodes << endl;
	      	  mv.print();
	      	}
	        else
	        {
		       	cin>>obj>>x1>>y1>>type>>x2>>y2;
		       	mv = Move (turn,x1+y1*M,x2+y2*M,(type=='B'));
	        }
		     	curr_state=curr_state.make_move(mv);
		     	if(turn==color){
		     		string str =curr_state.display();
		     		if(existed.find(str)==existed.end())
		     			existed.insert(pair<string,int>(str,1));
		     		else
		     			existed[str]++;
		     	}
		     	turn = -turn;
    }
}
