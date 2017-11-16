// Christian Panse <cp@fgcz.ethz.ch>
// 20171114 CNL470 ZH -> HAL 


#include <numeric>
#include <string>
#include <fstream>
#include <iostream>

#include <Rcpp.h>

using namespace Rcpp;

class Fasta {
public:
  Fasta(){}
  Fasta(const String filename): filename_(filename) {
    read();
  }
  
  /*
  StringVector getTrypticPeptides(){
    computeTrypticPeptides();
    return(Tryptic_);
  }
  */
  int getNumberOfTrypticPeptides(){
    computeTrypticPeptides();
    return(Tryptic_.size());
  }
  
  int getDesc() {
    return Desc_.size(); }
  int getSeq() {
    return Seq_.size(); }
  void addValue(int y) { x_ += y; }
  void merge(const Fasta& rhs) { x_ += rhs.x_; }
  
private:
  int x_;
  std::string filename_;
  std::vector<std::string> Desc_;
  std::vector<std::string> Seq_;
  std::vector<std::string> Tryptic_;
  
  void computeTrypticPeptides() {
    if (Tryptic_.size() > 0) return;
    // int n = 0;
    char aa0 = '\0';
    std::string digest = "";
    
    for (auto sequence : Seq_) {
      for (auto aa1 : sequence) {
        
        if (aa0 != '\0') {
          digest += aa0;
        }
        
        if ((aa1 != 'P' && aa0 == 'R') || aa0 == 'K') {
          if (digest != ""){
            Tryptic_.push_back(digest);
            digest = "";
          }
        }
        aa0 = aa1;
      }
    }
  }
  
  
  void read(){
    //filename_ = "/Users/cp/p1875_db10_20170817.fasta";
    
    std::string line;
    std::ifstream myfile (filename_);
    std::string s = "";
    
    if (myfile.is_open())
    {
      while ( std::getline (myfile, line) )
      {
        if (line[0] == '>') {
          Desc_.push_back(line);
          
          if (s.length() > 0) {
            Seq_.push_back(s);
            s = "";
          }
        }
        else {
          s = s.append(line);
        }
      }
      
      myfile.close();
      if (s.length() > 0)
        Seq_.push_back(s);
    }
  }
};


// Expose the classes
RCPP_MODULE(FastaMod) {
  //using namespace Rcpp;
  
  class_<Fasta>("Fasta")
  .default_constructor("Default constructor") // This exposes the default constructor
  .constructor<std::string>("sets initial value")
  .method("getDesc", &Fasta::getDesc, "Returns the value")
  .method("getSeq", &Fasta::getSeq, "Returns the value")
  .method("getNumberOfTrypticPeptides", &Fasta::getNumberOfTrypticPeptides, "Returns the value")
  .method("addValue", &Fasta::addValue, "Adds a value")
  .method("merge", &Fasta::merge, "Merges another Test into this object")
  ;
}

// .method("getTrypticPeptides", &Fasta::getTrypticPeptides, "Returns the value")