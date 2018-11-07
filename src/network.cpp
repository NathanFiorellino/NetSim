#include "network.h"
#include "random.h"
#include <random>



void Network::resize(const size_t& n){
	 
	values.clear();
	links.clear();
	/* We call this links.clear() so that in case we reduce the number 
	 * of nodes we don't have problems with links between non existing nodes*/
	
	for (size_t i(0); i<n ;++i){
		values.push_back(RNG.normal(0,1));
	}
}


bool Network::add_link(const size_t& a, const size_t& b){
	
	size_t min(std::min(a,b));
	size_t max(std::max(a,b));
	
	//Gestion of not Coherent links
	if( (a==b) or (max >= values.size()) ){
		return false;
		
	}else {
	
		for(auto it=links.begin(); it!=links.end(); ++it){
		//Here I use min and max to reduce the time needed to find both nodes
			if( ((*it).first == min) and ((*it).second == max)){
				return false;
			}
		}
		
		links.insert(std::pair<size_t,size_t>(a,b));
		links.insert(std::pair<size_t,size_t>(b,a));
		return true;
 	
 	}
}



size_t Network::random_connect(const double& n){
	
	links.clear();
	size_t totalLinksCreated(0);
	
	for(size_t i(0); i< values.size(); ++i){
		
		size_t degree(RNG.poisson(n));
		
		/* It may happen that the mean given by the user is 
		* near or even above the size of our values vector
		* Which means it can happen that degree is larger than
		* the possible links our node i can create
		* In this case we need to resize the degrees that are above 
		* our maximun number of links. But this means we will have less
		* links than predicted if our mean is near to our number of nodes*/
		
		if(degree>= size()){
			degree= size()-1;
		}	
		
		size_t linksCreated(0);
		 
		std::mt19937 rng;
		std::uniform_int_distribution<> distrib(0,values.size()-1);
		
		
		while(this->degree(i) < degree){
			if((add_link(i,distrib(rng))) ) {
				++linksCreated;
			}
		}

		totalLinksCreated +=linksCreated;
	}

	return totalLinksCreated;
}



size_t Network::set_values(const std::vector<double>& newValues){
	
	size_t size(std::min(newValues.size(),values.size()));
		
	for( size_t i(0); i<size; ++i){
		values[i]=newValues[i];
	}
	
  	return size;
}


size_t Network::size() const{
  	return  values.size();
}


size_t Network::degree(const size_t& n ) const{
	
	size_t degree(0);

 	for(auto it=links.begin(); it!=links.end(); ++it){
 		if((*it).first==n){
 			++degree;
 		}
 	}
 	
 	return degree;
}


 double Network::value(const size_t& n) const{
    return values[n];
}

std::vector<double> Network::sorted_values() const{ 

    std::vector<double> sortedValues (values);
    std::sort(sortedValues.begin(),sortedValues.end(),std::greater<double>());
    return sortedValues;
}

std::vector<size_t> Network::neighbors(const size_t& key) const{
	
    std::vector<size_t> neighbors;
    std::multimap<char,int>::iterator it;

	for(auto it=links.begin(); it!=links.end(); ++it){

		if( (*it).first == key){
       			neighbors.push_back((*it).second);
		}	
    }
    
  	return neighbors;
  }
