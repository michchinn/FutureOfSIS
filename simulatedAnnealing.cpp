#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <math.h>  
#include <stdlib.h> 
using namespace std;

#define large_num 9999999

//Returns the change in utility due to doing a hill climb swap
//If up, this should be positive
//If up is false, this should be negative
//If this returns 0, no more swaps can be done in given direction
//Prefs is student priority lists
//Assigned is a vector of course number, timeslot pairings
int numStudents;
int numClasses;
int hillClimb(vector<vector<int> >& prefs,vector<vector<pair<int, int> > >& assigned, bool up){
  vector<int> studentList;
  for(int s = 0; s < numStudents; s++){
    studentList.push_back(s);
  }
  random_shuffle(studentList.begin(),studentList.end());
  if(up){
    for(int h = 0; h < numStudents; h++){
      for(int k = 0; k < numStudents; k++){
	if(k == h){
	  continue;
	}
	int i = studentList[h];
	int j = studentList[k];
	
	if(assigned[i].size() == 0 && assigned[j].size() == 0){
	  continue;
	  //Can't swap if there is nothing to swap
	}
	
	int maxUtilDiff = -1;
	int iIdxToSwap = -1;
	int jIdxToSwap = -1;
	
	//First try swapping from j to i, nothing in return
	{
	  for(int idx = 0; idx < assigned[j].size(); idx++){
	    int Class = assigned[j][idx].first;
	    pair<int,int> toSwap = assigned[j][idx];
	    //cout << "Consider swapping " << Class << " from " << j << " to " << i << endl;
	    bool invalidSwap = false;
	    for(pair<int,int> timeSlot : assigned[i]){
	      if(timeSlot.second == toSwap.second){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == toSwap.first){
		invalidSwap = true;
		break;
	      }
	    }
	    if(invalidSwap){
	      continue;
	    }
	    int iUtil = numClasses - (find(prefs[i].begin(),prefs[i].end(),Class)-prefs[i].begin());
	    int jUtil = numClasses - (find(prefs[j].begin(),prefs[j].end(),Class)-prefs[j].begin());
	    if(iUtil - jUtil > maxUtilDiff){
	      jIdxToSwap = idx;
	      iIdxToSwap = -1;
	      maxUtilDiff = iUtil-jUtil;
	      //cout << "Max diff = " << maxUtilDiff << endl;
	    }
	  }  
	}
	//Now try swapping from i to j, nothing in return
	{
	  for(int idx = 0; idx < assigned[i].size(); idx++){
	    int Class = assigned[i][idx].first;
	    //cout << "Consider swapping " << Class << " from " << i << " to " << j << endl;
	    pair<int,int> toSwap = assigned[i][idx];
	    bool invalidSwap = false;
	    for(pair<int,int> timeSlot : assigned[j]){
	      if(timeSlot.second == toSwap.second){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == toSwap.first){
		invalidSwap = true;
		break;
	      }
	    }
	    if(invalidSwap){
	      continue;
	    }int iUtil = numClasses - (find(prefs[i].begin(),prefs[i].end(),Class)-prefs[i].begin());
	    int jUtil = numClasses - (find(prefs[j].begin(),prefs[j].end(),Class)-prefs[j].begin());
	    if(jUtil - iUtil > maxUtilDiff){
	      iIdxToSwap = idx;
	      jIdxToSwap = -1;
	      maxUtilDiff = jUtil-iUtil;
	      //cout << "Max diff = " << maxUtilDiff << endl;
	    }
	  }  
	}      
	//Now try doing a double swap
	for(int a = 0; a < assigned[i].size(); a++){
	  for(int b = 0; b < assigned[j].size(); b++){
	    pair<int,int> iClass = assigned[i][a];
	    pair<int,int> jClass = assigned[j][b];
	    
	    if(iClass.first == jClass.first){
	      continue;
	    }
	    bool invalidSwap = false;
	    for(pair<int,int> timeSlot : assigned[i]){
	      if(timeSlot.second == jClass.second && timeSlot.first != iClass.first){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == jClass.first){
		invalidSwap = true;
		break;
	      }
	    }
	    for(pair<int,int> timeSlot : assigned[j]){
	      if(timeSlot.second == iClass.second && timeSlot.first != jClass.first){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == iClass.first){
		invalidSwap = true;
		break;
	      }
	    }
	    if(invalidSwap){
	      continue;
	    }
	    
	    int iUtilGain = numClasses - (find(prefs[i].begin(),prefs[i].end(),jClass.first)-prefs[i].begin());
	    iUtilGain -= numClasses - (find(prefs[i].begin(),prefs[i].end(),iClass.first)-prefs[i].begin());
	    
	    int jUtilGain = numClasses - (find(prefs[j].begin(),prefs[j].end(),iClass.first)-prefs[j].begin());
	    jUtilGain -= numClasses - (find(prefs[j].begin(),prefs[j].end(),jClass.first)-prefs[j].begin());
	    
	    if(jUtilGain + iUtilGain > maxUtilDiff){
	      iIdxToSwap = a;
	      jIdxToSwap = b;
	      maxUtilDiff = jUtilGain + iUtilGain;
	      //cout << "Max diff = " << maxUtilDiff << endl;
	    }
	  }
	}
	if(iIdxToSwap == -1 && jIdxToSwap == -1){
	  //cout << "should this have happened?" << endl;
	  continue;
	}
	if(maxUtilDiff == 0){
	  continue;
	}
	if(iIdxToSwap == -1){
	  pair<int,int> classToMove = assigned[j][jIdxToSwap];
	  assigned[i].push_back(classToMove);
	  assigned[j].erase(assigned[j].begin()+jIdxToSwap);
	  //cout << "moving a class" << endl;
	  return maxUtilDiff;
	}
	else if(jIdxToSwap == -1){
	  pair<int,int> classToMove = assigned[i][iIdxToSwap];
	  assigned[j].push_back(classToMove);
	  assigned[i].erase(assigned[i].begin()+iIdxToSwap);
	  //cout << "moving a class" << endl;
	  return maxUtilDiff;
	}
	else{
	  pair<int,int> iToJ = assigned[i][iIdxToSwap];
	  pair<int,int> jToI = assigned[j][jIdxToSwap];
	  assigned[i].push_back(jToI);
	  assigned[j].push_back(iToJ);
	  assigned[i].erase(assigned[i].begin()+iIdxToSwap);
	  assigned[j].erase(assigned[j].begin()+jIdxToSwap);
	  return maxUtilDiff;
	}
      }
    }
  }
  else{
    for(int h = 0; h < numStudents; h++){
      for(int k = 0; k < numStudents; k++){
	if(k == h){
	  continue;
	}
	int i = studentList[h];
	int j = studentList[k];
	
	if(assigned[i].size() == 0 && assigned[j].size() == 0){
	  continue;
	  //Can't swap if there is nothing to swap
	}
	
	int minUtilDiff = 0;
	int iIdxToSwap = -1;
	int jIdxToSwap = -1;
	
	//First try swapping from j to i, nothing in return
	{
	  for(int idx = 0; idx < assigned[j].size(); idx++){
	    int Class = assigned[j][idx].first;
	    pair<int,int> toSwap = assigned[j][idx];
	    //cout << "Consider swapping " << Class << " from " << j << " to " << i << endl;
	    bool invalidSwap = false;
	    for(pair<int,int> timeSlot : assigned[i]){
	      if(timeSlot.second == toSwap.second){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == toSwap.first){
		invalidSwap = true;
		break;
	      }
	    }
	    if(invalidSwap){
	      continue;
	    }
	    int iUtil = numClasses - (find(prefs[i].begin(),prefs[i].end(),Class)-prefs[i].begin());
	    int jUtil = numClasses - (find(prefs[j].begin(),prefs[j].end(),Class)-prefs[j].begin());
	    if(iUtil - jUtil < minUtilDiff){
	      jIdxToSwap = idx;
	      iIdxToSwap = -1;
	      minUtilDiff = iUtil-jUtil;
	      //cout << "Max diff = " << maxUtilDiff << endl;
	    }
	  }  
	}
	//Now try swapping from i to j, nothing in return
	{
	  for(int idx = 0; idx < assigned[i].size(); idx++){
	    int Class = assigned[i][idx].first;
	    //cout << "Consider swapping " << Class << " from " << i << " to " << j << endl;
	    pair<int,int> toSwap = assigned[i][idx];
	    bool invalidSwap = false;
	    for(pair<int,int> timeSlot : assigned[j]){
	      if(timeSlot.second == toSwap.second){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == toSwap.first){
		invalidSwap = true;
		break;
	      }
	    }
	    if(invalidSwap){
	      continue;
	    }int iUtil = numClasses - (find(prefs[i].begin(),prefs[i].end(),Class)-prefs[i].begin());
	    int jUtil = numClasses - (find(prefs[j].begin(),prefs[j].end(),Class)-prefs[j].begin());
	    if(jUtil - iUtil < minUtilDiff){
	      iIdxToSwap = idx;
	      jIdxToSwap = -1;
	      minUtilDiff = jUtil-iUtil;
	      //cout << "Max diff = " << maxUtilDiff << endl;
	    }
	  }  
	}      
	//Now try doing a double swap
	for(int a = 0; a < assigned[i].size(); a++){
	  for(int b = 0; b < assigned[j].size(); b++){
	    pair<int,int> iClass = assigned[i][a];
	    pair<int,int> jClass = assigned[j][b];
	    
	    if(iClass.first == jClass.first){
	      continue;
	    }
	    bool invalidSwap = false;
	    for(pair<int,int> timeSlot : assigned[i]){
	      if(timeSlot.second == jClass.second && timeSlot.first != iClass.first){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == jClass.first){
		invalidSwap = true;
		break;
	      }
	    }
	    for(pair<int,int> timeSlot : assigned[j]){
	      if(timeSlot.second == iClass.second && timeSlot.first != jClass.first){
		invalidSwap = true;
		break;
	      }
	      else if(timeSlot.first == iClass.first){
		invalidSwap = true;
		break;
	      }
	    }
	    if(invalidSwap){
	      continue;
	    }
	    
	    int iUtilGain = numClasses - (find(prefs[i].begin(),prefs[i].end(),jClass.first)-prefs[i].begin());
	    iUtilGain -= numClasses - (find(prefs[i].begin(),prefs[i].end(),iClass.first)-prefs[i].begin());
	    
	    int jUtilGain = numClasses - (find(prefs[j].begin(),prefs[j].end(),iClass.first)-prefs[j].begin());
	    jUtilGain -= numClasses - (find(prefs[j].begin(),prefs[j].end(),jClass.first)-prefs[j].begin());
	    
	    if(jUtilGain + iUtilGain < minUtilDiff){
	      iIdxToSwap = a;
	      jIdxToSwap = b;
	      minUtilDiff = jUtilGain + iUtilGain;
	      //cout << "Max diff = " << maxUtilDiff << endl;
	    }
	  }
	}
	if(iIdxToSwap == -1 && jIdxToSwap == -1){
	  //cout << "should this have happened?" << endl;
	  continue;
	}
	if(minUtilDiff == 0){
	  continue;
	}
	if(iIdxToSwap == -1){
	  pair<int,int> classToMove = assigned[j][jIdxToSwap];
	  assigned[i].push_back(classToMove);
	  assigned[j].erase(assigned[j].begin()+jIdxToSwap);
	  //cout << "moving a class" << endl;
	  return minUtilDiff;
	}
	else if(jIdxToSwap == -1){
	  pair<int,int> classToMove = assigned[i][iIdxToSwap];
	  assigned[j].push_back(classToMove);
	  assigned[i].erase(assigned[i].begin()+iIdxToSwap);
	  //cout << "moving a class" << endl;
	  return minUtilDiff;
	}
	else{
	  pair<int,int> iToJ = assigned[i][iIdxToSwap];
	  pair<int,int> jToI = assigned[j][jIdxToSwap];
	  assigned[i].push_back(jToI);
	  assigned[j].push_back(iToJ);
	  assigned[i].erase(assigned[i].begin()+iIdxToSwap);
	  assigned[j].erase(assigned[j].begin()+jIdxToSwap);
	  return minUtilDiff;
	}
      }
    }
  }
  return 0;
}

int simulated_anneal(vector<vector<int> > students, vector<vector<pair<int, int> > >& assigned) {
	float Tmax = 25000.0;  //Max (starting) temperature
	float Tmin = 2.5;      // Min (ending) temperature
	int num_steps = 50000;   // Number of iterations
	int step = 1;
	float Tfactor = -log(Tmax / Tmin);
	int totalImprovement = 0;
	for(int i = 0; i < numStudents; i++){
		cout << "Student " << i << ": ";
		for(int sch = 0; sch < assigned[i].size(); sch++){
			cout << assigned[i][sch].first << "," << assigned[i][sch].second << " ";
		}
		cout << endl;
	}
	while (step < num_steps) {
		step++;
		float currTemp = Tmax * exp(Tfactor * step / num_steps);
		/** pseudo-simulated annealing. Typically, simulated annealing just accepts a random next state. Here
		* We are going to move randomly either closer or further from an optima, depending on a probability funciton
		* of the temperature and how great the difference hill climbing would produce.
		*/

		vector<vector<pair<int, int> > >& assignedCopyUp = assigned;
		vector<vector<pair<int, int> > >& assignedCopyDown = assigned;
	
		int upDiff = hillClimb(students,assignedCopyUp, true);
		int downDiff = hillClimb(students,assignedCopyDown, false);

	  bool acceptDownhill = exp(-downDiff / currTemp) < rand();
		bool acceptUphill =  exp(-upDiff / currTemp) < rand();
		if (acceptDownhill && !acceptUphill) {
			cout << "going downhill" << endl;
			assigned = assignedCopyDown;
			totalImprovement -= downDiff;
		} else if(acceptUphill ) {
		//s	cout << "going uphill" << endl;
			assigned = assignedCopyUp;
			totalImprovement += upDiff;
		} else {
			// do nothing
			cout << "not going up or downhill" << endl;
		}
			// choose either down or up

		}
	  for(int i = 0; i < numStudents; i++){
      cout << "Student " << i << ": ";
      for(int sch = 0; sch < assigned[i].size(); sch++){
				cout << assigned[i][sch].first << "," << assigned[i][sch].second << " ";
      }
      cout << endl;
    }
    	return totalImprovement;
	}





int main(){

  numStudents = 100;
  numClasses = 20;
  int numStudentsPerClass = 1;
  int numTimeSlots = 5;
  int numOfEachClass = 2;
  int numEnroll = 3;
  int hillClimbingIterations = 10000;
  /*
  numStudents = 200;
  numClasses = 8;
  int numStudentsPerClass = 20;
  int numTimeSlots = 5;
  int numOfEachClass = 2;
  int numEnroll = 3;
  int hillClimbingIterations = 10000;
  */
  
  //Initialize schedule 
  vector<int> classes;
  for(int i = 0; i < numClasses; i++){
    for(int c = 0; c < numOfEachClass; c++){
      classes.push_back(i);
    }
  }
  random_shuffle(classes.begin(),classes.end());
  
  vector<vector<pair<int,int> > > schedule;
  for(int i = 0; i < numTimeSlots; i++){
    vector<pair<int,int> > curr;
    for(int c = i; c < numOfEachClass*numClasses; c += numTimeSlots){
      curr.push_back(make_pair(classes[c],numStudentsPerClass));
    }
    schedule.push_back(curr);
  }
  
 
  for(int i = 0; i < numTimeSlots; i++){
    cout << "Time slot " << i << ": ";
    for(int c = 0; c < schedule[i].size(); c++){
      cout << schedule[i][c].first << "," << schedule[i][c].second << " ";
    }
    cout << endl;
  }

  //Initialize students
  vector<vector<int> > students;
  for(int i = 0; i < numStudents; i++){
    vector<int> curr;
    for(int c = 0; c < numClasses; c++){
      curr.push_back(c);
    }
    random_shuffle(curr.begin(),curr.end());
    students.push_back(curr);
    cout << "Student " << i << ": ";
    for(int c = 0; c < numClasses; c++){
      cout << students[i][c] << " ";
    }
    cout << endl;
  }
  cout << "-------------------------------" << endl;
  vector<vector<int> > schedules;
  vector<vector<pair<int,int> > > assigned;
  vector<int> utilities;
  
  int totalUtility = 0;
  for(int i = 0; i < numStudents; i++){
    
    vector<int> currSchedule;
    schedules.push_back(currSchedule);

    vector<pair<int, int> > currAssign;
    assigned.push_back(currAssign);

    int scheduled = 0;
    
    int currClassIdx = -1;
    utilities.push_back(0);
    
    while(currClassIdx < numClasses && scheduled < numEnroll){
      
      currClassIdx++;
      int currClass = students[i][currClassIdx];
      //cout << "Curr: " << currClass << endl;
     
      bool madeIt = false;

      for(int t = 0; t < numTimeSlots; t++){
	//cout << "T: " << t << endl;
	//Can only schedule when the student does not already have a class
	if( find(schedules[i].begin(),schedules[i].end(),t) == schedules[i].end() ){
	  for(int c = 0; c < schedule[t].size(); c++){
	    if(schedule[t][c].first == currClass && schedule[t][c].second > 0){
	      schedule[t][c].second--;
	      scheduled++;
	      schedules[i].push_back(t);
	      assigned[i].push_back(make_pair(currClass,t));
	      utilities[i] += numClasses - currClassIdx;
	      totalUtility += numClasses-currClassIdx;
	      madeIt = true;
	      //cout << "Student " << i << " placed into class " << currClass << endl;
	      break;
	    }
	  } 
	  if(madeIt){
	    break;
	  }
	}
      }
      
    }
    cout << "Student " << i << ": ";
    for(int sch = 0; sch < schedules[i].size(); sch++){
      cout << assigned[i][sch].first << "," << assigned[i][sch].second << " ";
    }
    cout << endl;
  }
  
  cout << "Total utility is: " << totalUtility << endl;
  	/*
  for(int hill = 0; hill < hillClimbingIterations; hill++){
    cout << "-------------------------------" << endl;
	
    int diff = hillClimb(students,assigned, false);
    for(int i = 0; i < numStudents; i++){
      cout << "Student " << i << ": ";
      for(int sch = 0; sch < assigned[i].size(); sch++){
	cout << assigned[i][sch].first << "," << assigned[i][sch].second << " ";
      }
      cout << endl;
    }
    if(diff == 0){
      cout << "Stopped climbing after " << hill << " iterations" << endl;
      break;
    }
    else if(diff < 0){
      cout << "diff is less than 0" << endl;
      totalUtility += diff;
    }
    else{
      cout << "diff is greater than 0" << endl;
      totalUtility += diff;
    }
    cout << "Utility is now " << totalUtility << endl;
	
  }
  
  cout << "Total utility is: " << totalUtility << endl;
	*/
	cout << "begin simulated anneal" << endl;
	totalUtility += simulated_anneal(students, assigned);
	cout << "total utility after anneal: " << totalUtility  << endl;

  return 0;
}

