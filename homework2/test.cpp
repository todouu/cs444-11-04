#include <iostream>
#include <string>

using namespace std;



struct philosopher
{
	int left;
	int right;
	string name;
	bool getting_left_frk; 
	bool getting_right_frk;
};



philosopher *arr_phil[] = {{4, 0, "Edsger Dijkstra", false, false}, 
			    {0, 1, "Harrison Ford", false, false},
				{1, 2, "Will Smith", false, false}, 
				{2, 3, "Gottfried Leibniz", false, false}, 
				{3, 4, "Garth Brooks", false, false}};  


void bla() {
		arr_phil[2].left = 9;
		arr_phil[2].right= 9;
		arr_phil[2].name = "haha";
		arr_phil[2].getting_left_frk = true;
		arr_phil[2].getting_right_frk = true;

}


int main(){
		for(int i = 0; i < 5 ; i++){
			cout << "name: " << arr_phil[i].name << endl;
			cout << "left: " << arr_phil[i].left<< endl;
			cout << "right: " << arr_phil[i].right<< endl;
		}							

		cout << endl;

		bla();

		for(int i = 0; i < 5 ; i++){
			cout << "name: " << arr_phil[i].name << endl;
			cout << "left: " << arr_phil[i].left<< endl;
			cout << "right: " << arr_phil[i].right<< endl;
		}	

	return 0;
}
