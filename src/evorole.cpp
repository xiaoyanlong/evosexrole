/*========================================================================================================
Individual variation in parental care drives divergence of female roles.cpp
==========================================================================================================

Written by:
Xiaoyan Long
Groningen Institute for Evolutionary Life Sciences (Gelifes)
University of Groningen
the Netherlands

Program version
20/07/2020

=========================================================================================================*/

//#include <array>
//
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <ctime>
//#include <random>
//#include <cassert>
//#include <chrono>
//#include <iomanip>
//#include <utility>
//#include <algorithm>
//#include <string>
//#include <cstdlib>
//#include "evorole.h"
//
//
//
///*============================= Model parameters ===================================*/
///* Model parameters: alleles at four gene loci */
//double varp;        // initial value of female preferences 
//double vart;        // initial value of male ornaments
//int    malepc;          // initail value of male care
//int    femalepc;      // initial value of female care
//
//
///* Model parameters: mortality costs */
//double mateMaleU;      // male mortality rate in the mating state 
//double mateFemaleU;      // female...
//double careMaleU;      // male mortality rate in the caring state
//double careFemaleU;      // female...
//double recMaleU;      // male mortality rate in the pre-mating state
//double recFemaleU;      // female
//double juvMaleU;      // male mortality rate in the juveniel state
//double juvFemaleU;      // female...
//double bmate;        // the intensity of stabilizing selection on ornamentation in the mating state
//double bcare;        // ... in the caring state
//double brec;        // ... in the pre-mating state
//
///* Model parameters: population */
//int     matePop;        // initialization for population size 
//double alphaFemale;      // degree of female density dependence
//double alphaMale;      // degree of male density dependence
//int     recoMaleT;      // the fixed days when males stay in the pre-mating state
//int     recoFemaleT;      // the fixed days when females stay in the pre-mating state
//int    juvT;        // the fixed days when juveniles stay in the juvenile and pre-mating state
//
///* Model parameters: sexual selection */
//double kappa;        // scaling factor that affects the intensity of sexual selection
//double alpha;        // scaling factor that affects the intensity of sexual selection
//
///* Model parameters: offspring survival */
//double demandOff;      // the care demand of offspring
//double syn;          // synergy parameter
//
///* Model parameters: mutation*/
//double  mp;          // mutational rate for gene p (preferences)
//double  signmap;      // sd step mutation size for p 
//double  pMaxmutstep;    // maximum mutation stepsize for p 
//double  mt;          // mutational rate for tau genes (ornaments)
//double  signmaTau;      // sd step mutation size for tau 
//double  tauMaxmutstep;    // maximum mutation stepsize for tau
//double  mpc;        // mutational rate for gene pc (parental care)
//
///* Model parameters: mate choice*/
//bool    ifRandom;           // if mating is random or not
//              // ifRandom = 0, mating is random and there is no sexual selection
//              // ifRandom = 1, female preference and male ornamemt coevolve with parental roles
//
///* Model parameters: simulation time*/
//int tEnd;
//
///* set seed */
//unsigned seed = static_cast <unsigned> (std::chrono::high_resolution_clock::now().time_since_epoch().count());
//std::mt19937_64 rng(seed);
//
///* globally distribution*/
//std::uniform_real_distribution <double> uniDis(0.0, 1.0);
//std::bernoulli_distribution female(0.5);  // 50% is female and 50% is male
//
///*============================= Model initialization ===================================*/
//
///*remove elements from the vector*/
//namespace detail {
//
//  // erases the element at position idx from container
//  // relative order behind idx is not preserved
//  // runtime complexity: O(1)
//  template <typename C, typename I>
//  void unstable_erase(C& container, I idx)
//  {
//    container[idx] = container.back();  // assuming self-assignment is fine
//    container.pop_back();
//  }
//
//
//  // erases the element at position idx from container
//  // relative order behind idx is preserved
//  // runtime complexity: O(conmtainer.size())
//  template <typename C, typename I>
//  void stable_erase(C& container, I idx)
//  {
//    container.erase(container.begin() + idx);
//  }
//
//}
//
///*Definition of the class: Individual*/
//class Individual {
//public:
//  Individual();
//  ~Individual() { };
//  double  getp() const { return (p); }
//  double  gettau() const { return (tau); }
//  int   getpc1() const { return (pc1); }
//  int     getpc2() const { return (pc2); }
//  int     getFemCare() const { return (femaleCareDay); }
//  int     getMaleCare() const { return (maleCareDay); }
//  std::pair <int, int> getpc1pc2()
//    const {
//    return std::pair <int, int>(pc1, pc2);
//  }
//  int    getFemaleRec() const { return(femaleRec); }
//  int    getMaleRec() const { return(maleRec); }
//  int    getjuvdays() const { return (juvdays); }
//  int     getmatingtimes() const { return (matetimes); }
//  int     getNumOff() const { return (numOff); }
//  int     getNumSurOff() const { return (numSurOff); }
//  void  writeFemaleRec(int&);
//  void  writeMaleRec(int&);
//  void  writejuvdays(int&);
//  void    writeFemaleCareDay(int&);
//  void    writeMaleCareDay(int&);
//  void    writeMateTimes(int&);
//  void    writeNumOff(int&);
//  void    writeNumSurOff(int&);
//  void  writegenome(double&, double&, std::vector <int>&);
//
//private:
//  double  p;          // individual preference, only expressed in females
//  double  tau;        // individual ornaments, only expressed in males
//  int    pc1;        // parental care only expressed in females
//  int    pc2;        // parental care only expressed in males
//  int    femaleRec;      // pre-mating period of the female
//  int    maleRec;      // pre-mating period of the male
//  int    juvdays;      // juvenile period of individuals
//  int     femaleCareDay;    // days left to provide parental care in female parents
//  int     maleCareDay;    // ... in male parents
//  int     matetimes;      // mating times of an individual (start from 0)
//  int     numOff;        // number of produced offspring of an individual
//  int     numSurOff;      // number of survival offspring of an individual
//};
//
///*Implementation of the class Individual*/
//Individual::Individual()
//{
//  p = varp;
//  tau = vart;
//  pc1 = femalepc;
//  pc2 = malepc;
//  femaleRec = recoFemaleT;
//  maleRec = recoMaleT;
//  juvdays = juvT;
//  femaleCareDay = pc1;
//  maleCareDay = pc2;
//  matetimes = 0;
//  numSurOff = 0;
//  numOff = 0;
//}
//
//void Individual::writegenome(double& P, double& T, std::vector <int>& vecPC)
//{
//  p = P;
//  tau = T;
//  pc1 = vecPC[0];
//  pc2 = vecPC[1];
//}
//
//
//void Individual::writeFemaleRec(int& newFemaleRec)
//{
//  femaleRec = newFemaleRec;
//}
//
//void Individual::writeMaleRec(int& newMaleRec)
//{
//  maleRec = newMaleRec;
//}
//void Individual::writejuvdays(int& newJuvdays)
//{
//  juvdays = newJuvdays;
//}
//
//void Individual::writeFemaleCareDay(int& newFemaleCareDay)
//{
//  femaleCareDay = newFemaleCareDay;
//}
//
//void Individual::writeMaleCareDay(int& newMaleCareDay)
//{
//  maleCareDay = newMaleCareDay;
//}
//
//void Individual::writeMateTimes(int& newwriteMateTimes)
//{
//  matetimes = newwriteMateTimes;
//}
//
//void Individual::writeNumOff(int& newNumOff)
//{
//  numOff = newNumOff;
//}
//
//void Individual::writeNumSurOff(int& newNumSurOff)
//{
//  numSurOff = newNumSurOff;
//}
//
//
///*mating, caing, pre-mating, juvenile state for males and females*/
//std::vector <Individual> mateMale;
//std::vector <Individual> mateFemale;
//std::vector <Individual> careMale;
//std::vector <Individual> careFemale;
//std::vector <Individual> recoMale;
//std::vector <Individual> recoFemale;
//std::vector <Individual> juvMale;
//std::vector <Individual> juvFemale;
//
//// output files
//std::ofstream allTrait;      // to record the population average value for all traits.
//std::ofstream offspringTrait;  // to record offspring trait 
//std::ofstream para;             // to record the parameters
//std::ofstream deadIndividual;   // to record all dead individuals
//
//
//
///*=============================== Start to simulate ===================================*/
//
//// average value for all traits (male care, female care, preferences, ornaments)
//void averageTrait()
//{
//  double sumFemalePC = 0.0;
//  double sumMalePC = 0.0;
//  double sumPreference = 0.0;
//  double sumOrnaments = 0.0;
//  for (int i = 0; i < mateMale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (mateMale[i].getpc1());
//    sumMalePC += static_cast <double> (mateMale[i].getpc2());
//    sumPreference += mateMale[i].getp();
//    sumOrnaments += mateMale[i].gettau();
//  }
//
//  for (int i = 0; i < careMale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (careMale[i].getpc1());
//    sumMalePC += static_cast <double> (careMale[i].getpc2());
//    sumPreference += careMale[i].getp();
//    sumOrnaments += careMale[i].gettau();
//  }
//
//  for (int i = 0; i < recoMale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (recoMale[i].getpc1());
//    sumMalePC += static_cast <double> (recoMale[i].getpc2());
//    sumPreference += recoMale[i].getp();
//    sumOrnaments += recoMale[i].gettau();
//  }
//
//  for (int i = 0; i < juvMale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (juvMale[i].getpc1());
//    sumMalePC += static_cast <double> (juvMale[i].getpc2());
//    sumPreference += juvMale[i].getp();
//    sumOrnaments += juvMale[i].gettau();
//  }
//
//  for (int i = 0; i < mateFemale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (mateFemale[i].getpc1());
//    sumMalePC += static_cast <double> (mateFemale[i].getpc2());
//    sumPreference += mateFemale[i].getp();
//    sumOrnaments += mateFemale[i].gettau();
//  }
//
//  for (int i = 0; i < careFemale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (careFemale[i].getpc1());
//    sumMalePC += static_cast <double> (careFemale[i].getpc2());
//    sumPreference += careFemale[i].getp();
//    sumOrnaments += careFemale[i].gettau();
//  }
//
//  for (int i = 0; i < recoFemale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (recoFemale[i].getpc1());
//    sumMalePC += static_cast <double> (recoFemale[i].getpc2());
//    sumPreference += recoFemale[i].getp();
//    sumOrnaments += recoFemale[i].gettau();
//  }
//
//  for (int i = 0; i < juvFemale.size(); ++i)
//  {
//    sumFemalePC += static_cast <double> (juvFemale[i].getpc1());
//    sumMalePC += static_cast <double> (juvFemale[i].getpc2());
//    sumPreference += juvFemale[i].getp();
//    sumOrnaments += juvFemale[i].gettau();
//  }
//
//
//  double popSize = static_cast <double> (mateMale.size()
//    + careMale.size() + recoMale.size()
//    + juvMale.size() + mateFemale.size()
//    + careFemale.size() + recoFemale.size()
//    + juvFemale.size());
//
//  sumFemalePC /= popSize;
//  sumMalePC /= popSize;
//  sumPreference /= popSize;
//  sumOrnaments /= popSize;
//  allTrait << ',' << sumFemalePC << ',' << sumMalePC << ',' << sumPreference << ',' << sumOrnaments;
//}
//
//double asrValue()
//{  // asr: male/(male+female), for all adults
//  double ASR = static_cast <double> (mateMale.size() + careMale.size() + recoMale.size())
//    / (static_cast <double> (mateFemale.size() + careFemale.size() + recoFemale.size() + mateMale.size()
//      + careMale.size() + recoMale.size()));
//  return ASR;
//}
//
//double osrValue()
//{  // osr: male/(male+female), for adults in the mating state
//  double OSR;
//  double mateSize = static_cast <double> (mateFemale.size() + mateMale.size());
//  if (mateSize != 0.0)
//    OSR = static_cast <double> (mateMale.size())
//    / static_cast <double> (mateFemale.size() + mateMale.size());
//  else
//    OSR = 0.5;
//  return OSR;
//}
//
//double psrValue()
//{  // psr: male/(male+female), for juveniles
//  double psr;
//  double juvSize = static_cast <double> (juvFemale.size() + juvMale.size());
//  if (juvSize != 0.0)
//    psr = static_cast <double> (juvMale.size()) / juvSize;
//  else psr = 0.5;
//  return psr;
//}
//
//// traits mutation
//void addmutationp(double& val) { // to add mutation for alleles p
//  std::bernoulli_distribution ismutation(mp);
//  if (ismutation(rng)) {
//
//    std::cauchy_distribution <double> dis(0.0, signmap);
//
//    double mut = dis(rng);
//
//    if (abs(mut) > pMaxmutstep) {
//
//      if (mut < 0.0)
//        mut = 0.0 - pMaxmutstep;
//      else
//        mut = pMaxmutstep;
//    }
//    val += mut;
//    if (val < 0.0) {
//      val = 0.0;
//    }
//  }
//}
//
//void addmutationTau(double& val) { // to add mutation for alleles t
//  std::bernoulli_distribution ismutation(mt);
//  if (ismutation(rng)) {
//    std::cauchy_distribution <double> dis(0.0, signmaTau);
//    double mut = dis(rng);
//
//    if (abs(mut) > tauMaxmutstep) {
//
//      if (mut < 0.0)
//        mut = 0.0 - tauMaxmutstep;
//      else
//        mut = tauMaxmutstep;
//    }
//    val += mut;
//
//    if (val < 0.0) {
//      val = 0.0;
//    }
//  }
//}
//
//void addmutationPC(int& val) { // to add mutation for alleles pc
//  std::bernoulli_distribution ismutation(mpc);
//  if (ismutation(rng)) {
//    std::bernoulli_distribution isPlus(0.5);
//    if (isPlus(rng))
//      val++;
//    else val--;
//    if (val < 0) {
//      val = 0;
//    }
//  }
//}
//
//Individual madeOff(const Individual& fem, const Individual& male) {
//  // function to create an offspring by its female and male
//
//  double newp;            // offspring preferences
//  double newt;            // offspring ornaments
//  std::vector <int> newpc(2, 0);    // offspring parental care
//  int newFemaleCareDays;        // maternal care
//  int newMaleCareDays;        // paternal care
//
//  std::bernoulli_distribution Nallele(0.5); // 50% pro to get gene from mom, 50% from dad
//
//  // select new alleles p
//  double pfem = fem.getp();
//  double pmale = male.getp();
//  int x1 = Nallele(rng);
//  switch (x1) {
//  case 0:
//    newp = pfem;
//    break;
//  case 1:
//    newp = pmale;
//    break;
//  default:
//    std::cout << " something wronk in chosing allele pc for offspring";
//    break;
//  }
//  addmutationp(newp);
//
//  // select new alleles t (the same as p)
//  double tfem = fem.gettau();
//  double tmale = male.gettau();
//  int x2 = Nallele(rng);
//  switch (x2) {
//  case 0:
//    newt = tfem;
//    break;
//  case 1:
//    newt = tmale;
//    break;
//  default:
//    std::cout << " something wronk in chosing allele t for offspring";
//    break;
//  }
//  addmutationTau(newt);
//
//  // select new alleles pc1 (the same as p)
//  std::pair <int, int> pcfem = fem.getpc1pc2();
//  std::pair <int, int> pcmal = male.getpc1pc2();
//  int x3 = Nallele(rng);
//  switch (x3) {
//  case 0:
//    newpc[0] = pcfem.first;
//    break;
//  case 1:
//    newpc[0] = pcmal.first;
//    break;
//  default:
//    std::cout << " something wronk in chosing allele pc for offspring"; // in case doesn't work.
//    break;
//  }
//  addmutationPC(newpc[0]);
//
//  // select new alleles pc2 (the same as p)
//  int x4 = Nallele(rng);
//  switch (x4) {
//  case 0:
//    newpc[1] = pcfem.second;
//    break;
//  case 1:
//    newpc[1] = pcmal.second;
//    break;
//  default:
//    std::cout << " something wronk in chosing allele pc for offspring"; // in case doesn't work.
//    break;
//  }
//  addmutationPC(newpc[1]);
//
//  Individual newOff;
//  newOff.writegenome(newp, newt, newpc);
//  newOff.writeFemaleCareDay(newpc[0]);
//  newOff.writeMaleCareDay(newpc[1]);
//  return newOff;
//}
//
//int lotteryMale(const std::vector <Individual>& male)
//{
//  std::vector <double> mortalityRates(male.size());
//  for (int i = 0; i < mortalityRates.size(); ++i)
//    mortalityRates[i] = mateMaleU + bmate * male[i].gettau() * male[i].gettau();
//  std::discrete_distribution <int> lottM(mortalityRates.begin(), mortalityRates.end());
//  return lottM(rng);
//}
//
//int selectIndividual(std::vector<Individual>& vec) {
//  std::uniform_int_distribution <int> fDis(0, static_cast<int>(vec.size()) - 1);
//  int j = fDis(rng);
//  return j;
//}
//
//// mating state: check how many individual would die in the mating state every time step
//void checkMatingDeath(const int& t)
//{
//  // check the death of males (if there is sexual selection, males with larger ornaments have higher probability of dying)
//  if (mateMale.size() > 0) {
//    double sumMaleDeath = 0.0;
//    for (int i = 0; i < mateMale.size(); ++i)
//      sumMaleDeath += mateMaleU + bmate * mateMale[i].gettau() * mateMale[i].gettau();
//    std::poisson_distribution <int> maleDeath(sumMaleDeath);
//    int numMaleDeath = maleDeath(rng);
//
//    if (numMaleDeath > 0)
//    {
//      int m;
//      for (int i = 0; i < numMaleDeath; ++i)
//      {
//        m = lotteryMale(mateMale);
//        if (t % 40000 == 0)
//        {
//          deadIndividual << t << ',' << "matemale" << ',' << ',' << mateMale[m].getp() << ',' << mateMale[m].gettau() << ',' << mateMale[m].getpc1() << ','
//            << mateMale[m].getpc2() << ',' << mateMale[m].getmatingtimes() << ',' << mateMale[m].getNumOff() << ',' << mateMale[m].getNumSurOff() << std::endl;
//        }
//        detail::unstable_erase(mateMale, m);
//      }
//    }
//  }
//  // check the death of females
//  if (mateFemale.size() > 0) {
//    double sumFemaleDeath = mateFemaleU * static_cast <double> (mateFemale.size());
//    std::poisson_distribution <int> femaleDeath(sumFemaleDeath);
//    int numFemaleDeath = femaleDeath(rng);
//    if (numFemaleDeath > 0)
//    {
//      int f;
//      for (int i = 0; i < numFemaleDeath; ++i)
//      {
//        f = selectIndividual(mateFemale);
//        if (t % 40000 == 0)
//        {
//          deadIndividual << t << ',' << "matefemale" << ',' << ',' << mateFemale[f].getp() << ',' << mateFemale[f].gettau() << ',' << mateFemale[f].getpc1() << ','
//            << mateFemale[f].getpc2() << ',' << mateFemale[f].getmatingtimes() << ',' << mateFemale[f].getNumOff() << ',' << mateFemale[f].getNumSurOff() << std::endl;
//        }
//        detail::unstable_erase(mateFemale, f);
//      }
//    }
//  }
//}
//
//void ifmate(const int& femID, const int& maleID, int& t)
//{ // the mated pairs switch to the caring state and produce offspring
//  int newmatingFT = mateFemale[femID].getmatingtimes() + 1;
//  int newmatingMT = mateMale[maleID].getmatingtimes() + 1;
//  mateFemale[femID].writeMateTimes(newmatingFT);
//  mateMale[maleID].writeMateTimes(newmatingMT);
//  double maleCareT;
//  double femaleCareT;
//  double surProMale = pow((1.0 - (careMaleU + bcare * mateMale[maleID].gettau() * mateMale[maleID].gettau())),
//    static_cast <double> (mateMale[maleID].getpc2()));
//  std::bernoulli_distribution surMale(surProMale);
//  if (surMale(rng))
//  {
//    maleCareT = static_cast <double> (mateMale[maleID].getpc2());
//    careMale.push_back(mateMale[maleID]);
//  }
//  else
//  {  // if the male would die during providing care, expected care duration would be calculated
//    double malePCtau = (1.0 / careMaleU) -
//      (mateMale[maleID].getpc2() / (exp(careMaleU * mateMale[maleID].getpc2()) - 1.0));
//    maleCareT = malePCtau;
//    if (t % 40000 == 0)
//    {
//      deadIndividual << t << ',' << "caremale" << ',' << ',' << mateMale[maleID].getp() << ',' << mateMale[maleID].gettau() << ',' << mateMale[maleID].getpc1() << ','
//        << mateMale[maleID].getpc2() << ',' << mateMale[maleID].getmatingtimes() << ',' << mateMale[maleID].getNumOff() << ',' << mateMale[maleID].getNumSurOff() << std::endl;
//    }
//  }
//
//  // the same step for the female
//  double surProFemale = pow((1.0 - careFemaleU), static_cast <double> (mateFemale[femID].getpc1()));
//  std::bernoulli_distribution surFemale(surProFemale);
//  if (surFemale(rng))
//  {
//    femaleCareT = static_cast <double> (mateFemale[femID].getpc1());
//    careFemale.push_back(mateFemale[femID]);
//  }
//  else
//  {
//    double femalePCtau = (1.0 / careFemaleU) - (mateFemale[femID].getpc1() /
//      (exp(careFemaleU * mateFemale[femID].getpc1()) - 1.0));
//    femaleCareT = femalePCtau;
//    if (t % 40000 == 0)
//    {
//      deadIndividual << t << ',' << "carefemale" << ',' << mateFemale[femID].getp() << ',' << mateFemale[femID].gettau() << ',' << mateFemale[femID].getpc1() << ','
//        << mateFemale[femID].getpc2() << ',' << mateFemale[femID].getmatingtimes() << ',' << mateFemale[femID].getNumOff() << ',' << mateFemale[femID].getNumSurOff() << std::endl;
//    }
//  }
//
//  // the male and the female produce offspring after mating, the offspring survival probability depends on the total care duration
//  double totalCare = maleCareT + femaleCareT + maleCareT * femaleCareT * syn;
//  double surPro = (totalCare * totalCare) / (totalCare * totalCare + demandOff * demandOff);
//  if (surPro > uniDis(rng)) {
//    int newNumMaleOff = mateMale[maleID].getNumOff() + 1;
//    int newNumFemeOff = mateFemale[femID].getNumOff() + 1;
//    mateMale[maleID].writeNumOff(newNumMaleOff);
//    mateFemale[femID].writeNumOff(newNumFemeOff);
//  }
//  double popSize = static_cast <double> (mateMale.size() + mateFemale.size() + careMale.size()
//    + careFemale.size() + recoMale.size() + recoFemale.size() + juvMale.size() + juvFemale.size());
//  double breedSucFem = surPro / (1.0 + alphaFemale * popSize);
//  double breedSucMale = surPro / (1.0 + alphaMale * popSize);
//
//  // write the days that offspring should stay with their parents 
//  int realmalecareDays = static_cast <int> (ceil(maleCareT)) + juvT;  // we use ceil value, e.g.: 5.2-->6
//  int realfemalecareDays = static_cast <int> (ceil(femaleCareT)) + juvT;
//  int x = female(rng);
//  switch (x)
//  {
//  case 0:
//    // if the offspring is a female
//    if (uniDis(rng) < breedSucFem && uniDis(rng) < pow((1.0 - juvFemaleU), static_cast <double> (juvT)))
//    {
//      // if the offspring still survives after juvenile and pre-mating state
//      int newNumSurMaleOff = mateMale[maleID].getNumSurOff() + 1;
//      int newNumSurFemeOff = mateFemale[femID].getNumSurOff() + 1;
//      mateMale[maleID].writeNumSurOff(newNumSurMaleOff);
//      mateFemale[femID].writeNumSurOff(newNumSurFemeOff);
//      Individual newFelOffspring = madeOff(mateFemale[femID], mateMale[maleID]);
//      if (realfemalecareDays > realmalecareDays) {
//        newFelOffspring.writejuvdays(realfemalecareDays);
//      }
//      else {
//        newFelOffspring.writejuvdays(realmalecareDays);
//      }
//      juvFemale.push_back(newFelOffspring);
//      if (t % 40000 == 0) {
//        offspringTrait << t << ',' << "female" << ',' << mateMale[maleID].getp() << ',' << mateMale[maleID].gettau() << ','
//          << mateMale[maleID].getpc1() << ',' << mateMale[maleID].getpc2() << ','
//          << mateFemale[femID].getp() << ',' << mateFemale[femID].gettau() << ',' << mateFemale[femID].getpc1() << ','
//          << mateFemale[femID].getpc2() << ','
//          << juvFemale.back().getp() << ',' << juvFemale.back().gettau() << ',' << juvFemale.back().getpc1() << ','
//          << juvFemale.back().getpc2() << ',' << juvFemale.back().getjuvdays() << std::endl;
//
//      }
//    }
//    break;
//  case 1:
//    // if the offspring is a male
//    if (uniDis(rng) < breedSucMale && uniDis(rng) < pow((1.0 - juvMaleU), static_cast <double> (juvT)))
//    {
//      int newNumSurMaleOff = mateMale[maleID].getNumSurOff() + 1;
//      int newNumSurFemeOff = mateFemale[femID].getNumSurOff() + 1;
//      mateMale[maleID].writeNumSurOff(newNumSurMaleOff);
//      mateFemale[femID].writeNumSurOff(newNumSurFemeOff);
//      Individual newMaleOffspring = madeOff(mateFemale[femID], mateMale[maleID]);
//      if (realfemalecareDays > realmalecareDays) {
//        newMaleOffspring.writejuvdays(realfemalecareDays);
//      }
//      else {
//        newMaleOffspring.writejuvdays(realmalecareDays);
//      }
//      juvMale.push_back(newMaleOffspring);
//      if (t % 40000 == 0) {
//        offspringTrait << t << ',' << "male" << ',' << mateMale[maleID].getp() << ',' << mateMale[maleID].gettau() << ','
//          << mateMale[maleID].getpc1() << ',' << mateMale[maleID].getpc2() << ','
//          << mateFemale[femID].getp() << ',' << mateFemale[femID].gettau() << ',' << mateFemale[femID].getpc1() << ','
//          << mateFemale[femID].getpc2() << ','
//          << juvMale.back().getp() << ',' << juvMale.back().gettau() << ',' << juvMale.back().getpc1() << ','
//          << juvMale.back().getpc2() << ',' << juvMale.back().getjuvdays() << std::endl;
//      }
//    }
//    break;
//  default:
//    std::cout << "Something wrong with producing offspring" << std::endl;
//    break;
//  }
//  detail::unstable_erase(mateMale, maleID);
//  detail::unstable_erase(mateFemale, femID);
//}
//
//
//void getCaringDays(int& t)
//{  // individuals seek mating chances, produce offspring and provide parental care 
//  checkMatingDeath(t);
//
//  if (mateMale.size() > 0 && mateFemale.size() > 0)
//  {
//    std::shuffle(mateFemale.begin(), mateFemale.end(), rng);
//    if (ifRandom == 0) { // in the mating state, mating is random 
//      for (int i = 0; i < mateFemale.size(); ++i)
//      {
//        if (mateMale.size() > 0)
//        {
//
//          int m = selectIndividual(mateMale);
//          ifmate(i, m, t);
//          --i;
//        }
//        else break;
//      }
//    }
//    else
//    {  // if sexual selection is considered
//      for (int i = 0; i < mateFemale.size(); ++i)
//      {
//        if (mateMale.size() > 0) {
//          double p = mateFemale[i].getp();
//          int m = selectIndividual(mateMale);
//          double s = mateMale[m].gettau();
//          double attrac = 1.0 / (1.0 + kappa * exp(alpha * (p - s)));
//          if (uniDis(rng) < attrac)
//          {  // if the male is attractive to the female, the female would mate with the male
//            ifmate(i, m, t);
//            --i;
//          }
//        }
//        else break;
//      }
//    }
//  }
//}
//
//
//void gotoRecover(const int& t)
//{  // individuals enter the pre-mating state after caring 
//  // (no pre-mating in the baseline model)
//  for (int i = 0; i < careMale.size();)
//  {
//    int malePC = careMale[i].getMaleCare();
//    if (malePC > 0)
//    {
//      --malePC;
//      careMale[i].writeMaleCareDay(malePC);
//      ++i;
//    }
//    else
//    {
//      double surProMale = pow((1.0 - (recMaleU + brec * careMale[i].gettau() * careMale[i].gettau())),
//        static_cast <double> (recoMaleT));
//
//      std::bernoulli_distribution surMale(surProMale);
//      if (surMale(rng))
//      {  // if the male still survives after caring, he enter the pre-mating state
//        int maleCareDays = careMale[i].getpc2();
//        careMale[i].writeMaleCareDay(maleCareDays);
//        recoMale.push_back(careMale[i]);
//      }
//      detail::unstable_erase(careMale, i);
//    }
//  }
//
//  for (int i = 0; i < careFemale.size();)
//  {
//    int femalePC = careFemale[i].getFemCare();
//    if (femalePC > 0)
//    {
//      --femalePC;
//      careFemale[i].writeFemaleCareDay(femalePC);
//      ++i;
//    }
//    else
//    {
//      double surProFemale = pow((1.0 - recFemaleU), static_cast <double> (recoFemaleT));
//      std::bernoulli_distribution surFemale(surProFemale);
//      if (surFemale(rng))
//      {
//        int femaleCareDays = careFemale[i].getpc1();
//        careFemale[i].writeFemaleCareDay(femaleCareDays);
//        recoFemale.push_back(careFemale[i]);
//      }
//      detail::unstable_erase(careFemale, i);
//    }
//  }
//}
//
//void backtoMating(const int& t)
//{  // Individuals come back to the mating state after pre-mating
//  for (int i = 0; i < recoMale.size();)
//  {
//    int maleRec = recoMale[i].getMaleRec();
//    if (maleRec > 0)
//    {
//      --maleRec;
//      recoMale[i].writeMaleRec(maleRec);
//      ++i;
//    }
//    else
//    {
//      recoMale[i].writeMaleRec(recoMaleT);
//      mateMale.push_back(recoMale[i]);
//      detail::unstable_erase(recoMale, i);
//    }
//  }
//
//  for (int i = 0; i < recoFemale.size();)
//  {
//    int femaleRec = recoFemale[i].getFemaleRec();
//    if (femaleRec > 0)
//    {
//      --femaleRec;
//      recoFemale[i].writeFemaleRec(femaleRec);
//      ++i;
//    }
//    else
//    {
//      recoFemale[i].writeFemaleRec(recoFemaleT);
//      mateFemale.push_back(recoFemale[i]);
//      detail::unstable_erase(recoFemale, i);
//    }
//  }
//}
//
//void juvToMating()
//{  // juveniles enter the mating state after getting mature
//  for (int i = 0; i < juvMale.size();)
//  {
//    int juvpoolT = juvMale[i].getjuvdays();
//    if (juvpoolT > 0)
//    {
//      --juvpoolT;
//      juvMale[i].writejuvdays(juvpoolT);
//      ++i;
//    }
//    else
//    {
//      juvMale[i].writejuvdays(juvT);
//      mateMale.push_back(juvMale[i]);
//      detail::unstable_erase(juvMale, i);
//    }
//  }
//
//  for (int i = 0; i < juvFemale.size();)
//  {
//    int juvpoolT = juvFemale[i].getjuvdays();
//    if (juvpoolT > 0)
//    {
//      --juvpoolT;
//      juvFemale[i].writejuvdays(juvpoolT);
//      ++i;
//    }
//    else
//    {
//      juvFemale[i].writejuvdays(juvT);
//      mateFemale.push_back(juvFemale[i]);
//      detail::unstable_erase(juvFemale, i);
//    }
//  }
//}
//
//void refreshVectors() {
//  mateMale = std::vector <Individual>(matePop / 2);
//  mateFemale = std::vector <Individual>(matePop / 2);
//  careMale = std::vector <Individual>(0);
//  careFemale = std::vector <Individual>(0);
//  recoMale = std::vector <Individual>(0);
//  recoFemale = std::vector <Individual>(0);
//  juvMale = std::vector <Individual>(0);
//  juvFemale = std::vector <Individual>(0);
//
//}
//
//void readParamaters(const std::string& parFileName)
//{
//  std::ifstream ifs(parFileName.c_str(), std::ios::in);
//  if (!ifs.is_open()) {
//    std::cerr << "error: unable to open parameter file " << parFileName << '\n';
//    exit(EXIT_FAILURE);
//  }
//  std::clog << "reading parameters from file " << parFileName << '\n';
//  for (;;) {
//    // read a parameter name
//    std::string parID;
//    ifs >> parID;
//    if (ifs.good()) {
//      // read in parameter value in the appropriate way
//      if (parID == "varp") {
//        ifs >> varp;
//        std::clog << "parameter " << parID << " set to " << varp << '\n';
//      }
//      else if (parID == "vart") {
//        ifs >> vart;
//        std::clog << "parameter " << parID << " set to " << vart << '\n';
//      }
//      else if (parID == "malepc") {
//        ifs >> malepc;
//        std::clog << "parameter " << parID << " set to " << malepc << '\n';
//      }
//      else if (parID == "femalepc") {
//        ifs >> femalepc;
//        std::clog << "parameter " << parID << " set to " << femalepc << '\n';
//      }
//      else if (parID == "mateMaleU") {
//        ifs >> mateMaleU;
//        std::clog << "parameter " << parID << " set to " << mateMaleU << '\n';
//      }
//      else if (parID == "mateFemaleU") {
//        ifs >> mateFemaleU;
//        std::clog << "parameter " << parID << " set to " << mateFemaleU << '\n';
//      }
//      else if (parID == "careMaleU") {
//        ifs >> careMaleU;
//        std::clog << "parameter " << parID << " set to " << careMaleU << '\n';
//      }
//      else if (parID == "careFemaleU") {
//        ifs >> careFemaleU;
//        std::clog << "parameter " << parID << " set to " << careFemaleU << '\n';
//      }
//      else if (parID == "recMaleU") {
//        ifs >> recMaleU;
//        std::clog << "parameter " << parID << " set to " << recMaleU << '\n';
//      }
//      else if (parID == "recFemaleU") {
//        ifs >> recFemaleU;
//        std::clog << "parameter " << parID << " set to " << recFemaleU << '\n';
//      }
//      else if (parID == "juvMaleU") {
//        ifs >> juvMaleU;
//        std::clog << "parameter " << parID << " set to " << juvMaleU << '\n';
//      }
//      else if (parID == "juvFemaleU") {
//        ifs >> juvFemaleU;
//        std::clog << "parameter " << parID << " set to " << juvFemaleU << '\n';
//      }
//      else if (parID == "bmate") {
//        ifs >> bmate;
//        std::clog << "parameter " << parID << " set to " << bmate << '\n';
//      }
//      else if (parID == "bcare") {
//        ifs >> bcare;
//        std::clog << "parameter " << parID << " set to " << bcare << '\n';
//      }
//      else if (parID == "brec") {
//        ifs >> brec;
//        std::clog << "parameter " << parID << " set to " << brec << '\n';
//      }
//      else if (parID == "matePop") {
//        ifs >> matePop;
//        std::clog << "parameter " << parID << " set to " << matePop << '\n';
//      }
//      else if (parID == "alphaFemale") {
//        ifs >> alphaFemale;
//        std::clog << "parameter " << parID << " set to " << alphaFemale << '\n';
//      }
//      else if (parID == "alphaMale") {
//        ifs >> alphaMale;
//        std::clog << "parameter " << parID << " set to " << alphaMale << '\n';
//      }
//      else if (parID == "recoMaleT") {
//        ifs >> recoMaleT;
//        std::clog << "parameter " << parID << " set to " << recoMaleT << '\n';
//      }
//      else if (parID == "recoFemaleT") {
//        ifs >> recoFemaleT;
//        std::clog << "parameter " << parID << " set to " << recoFemaleT << '\n';
//      }
//      else if (parID == "juvT") {
//        ifs >> juvT;
//        std::clog << "parameter " << parID << " set to " << juvT << '\n';
//      }
//      else if (parID == "kappa") {
//        ifs >> kappa;
//        std::clog << "parameter " << parID << " set to " << kappa << '\n';
//      }
//      else if (parID == "alpha") {
//        ifs >> alpha;
//        std::clog << "parameter " << parID << " set to " << alpha << '\n';
//      }
//      else if (parID == "demandOff") {
//        ifs >> demandOff;
//        std::clog << "parameter " << parID << " set to " << demandOff << '\n';
//      }
//      else if (parID == "syn") {
//        ifs >> syn;
//        std::clog << "parameter " << parID << " set to " << syn << '\n';
//      }
//      else if (parID == "mp") {
//        ifs >> mp;
//        std::clog << "parameter " << parID << " set to " << mp << '\n';
//      }
//      else if (parID == "signmap") {
//        ifs >> signmap;
//        std::clog << "parameter " << parID << " set to " << signmap << '\n';
//      }
//      else if (parID == "pMaxmutstep") {
//        ifs >> pMaxmutstep;
//        std::clog << "parameter " << parID << " set to " << pMaxmutstep << '\n';
//      }
//      else if (parID == "mt") {
//        ifs >> mt;
//        std::clog << "parameter " << parID << " set to " << mt << '\n';
//      }
//      else if (parID == "signmaTau") {
//        ifs >> signmaTau;
//        std::clog << "parameter " << parID << " set to " << signmaTau << '\n';
//      }
//      else if (parID == "tauMaxmutstep") {
//        ifs >> tauMaxmutstep;
//        std::clog << "parameter " << parID << " set to " << tauMaxmutstep << '\n';
//      }
//      else if (parID == "mpc") {
//        ifs >> mpc;
//        std::clog << "parameter " << parID << " set to " << mpc << '\n';
//      }
//
//      else if (parID == "ifRandom") {
//        ifs >> ifRandom;
//        std::clog << "parameter " << parID << " set to " << ifRandom << '\n';
//      }
//      else if (parID == "tEnd") {
//        ifs >> tEnd;
//        std::clog << "parameter " << parID << " set to " << tEnd << '\n';
//      }
//      else {
//        std::cerr << "error: unknown parameter name in file " << parFileName << '\n';
//        exit(EXIT_FAILURE);
//      }
//    }
//    else break; // to exit when EOF is reached
//  }
//}
//
//void recordParameter() { // write parameter on the output file
//  para << "  Seed = " << seed << "\n";
//  para << " varp" << ',' << varp << "\n";
//  para << " vart" << ',' << vart << "\n";
//  para << " malepcmin  " << ',' << malepc << "\n";
//  para << " femalepcmin " << ',' << femalepc << "\n";
//  para << " mateMaleU    " << ',' << mateMaleU << "\n";
//  para << " mateFemaleU  " << ',' << mateFemaleU << "\n";
//  para << " careMaleU    " << ',' << careMaleU << "\n";
//  para << " careFemaleU  " << ',' << careFemaleU << "\n";
//  para << " recMaleU    " << ',' << recMaleU << "\n";
//  para << " recFemaleU  " << ',' << recFemaleU << "\n";
//  para << " juvMaleU    " << ',' << juvMaleU << "\n";
//  para << " juvFemaleU  " << ',' << juvFemaleU << "\n";
//  para << " bmate      " << ',' << bmate << "\n";
//  para << " bcare      " << ',' << bcare << "\n";
//  para << " brec      " << ',' << brec << "\n";
//  para << " matePop    " << ',' << matePop << "\n";
//  para << " alphaFemale  " << ',' << alphaFemale << "\n";
//  para << " alphaMale    " << ',' << alphaMale << "\n";
//  para << " recoMaleT  " << ',' << recoMaleT << "\n";
//  para << " recoFemaleT  " << ',' << recoFemaleT << "\n";
//  para << " juvT      " << ',' << juvT << "\n";
//  para << " kappa      " << ',' << kappa << "\n";
//  para << " alpha      " << ',' << alpha << "\n";
//  para << " demandOff  " << ',' << demandOff << "\n";
//  para << " syn      " << ',' << syn << "\n";
//  para << " mp      " << ',' << mp << "\n";
//  para << " signmap    " << ',' << signmap << "\n";
//  para << " pMaxmutstep  " << ',' << pMaxmutstep << "\n";
//  para << " mt      " << ',' << mt << "\n";
//  para << " signmaTau    " << ',' << signmaTau << "\n";
//  para << " tauMaxmutstep  " << ',' << tauMaxmutstep << "\n";
//  para << " mpc      " << ',' << mpc << "\n";
//  para << " ifRandom      " << ',' << ifRandom << "\n";
//  para << " tEnd      " << ',' << tEnd << "\n";
//}
//
//
//
//int main(int argc, const char* argv[])
//{
//  for (int file_number = 1; file_number <= 2; ++file_number) {
//    std::clog << "starting program" << argv[0] << '\n';
//
//    // extract parameterfilename from program arguments or use default
//    std::string fileName = "parameter_" + std::to_string(file_number) + ".txt";
//    std::string parameterFileName(fileName);
//    if (argc > 1)
//      parameterFileName = argv[1];
//    readParamaters(parameterFileName);
//    for (int simulation_number = 1; simulation_number <= 100; ++simulation_number)
//    {
//
//      refreshVectors();
//
//      std::string fileName1 = "1parameter_" + std::to_string(file_number) + "_Parental_Care_" + std::to_string(simulation_number) + ".csv";
//      std::string fileName2 = "1parameter_" + std::to_string(file_number) + "_individual_" + std::to_string(simulation_number) + ".csv";
//      std::string fileName3 = "1parameter_" + std::to_string(file_number) + "_parameter_" + std::to_string(simulation_number) + ".csv";
//      std::string fileName4 = "1parameter_" + std::to_string(file_number) + "_deadIndividual_" + std::to_string(simulation_number) + ".csv";
//
//      allTrait.open(fileName1, std::ios_base::ate);
//      offspringTrait.open(fileName2, std::ios_base::ate);
//      para.open(fileName3, std::ios_base::ate);
//      deadIndividual.open(fileName4, std::ios_base::ate);
//      recordParameter();
//
//      allTrait << "day" << ',' << "osr" << ',' << "asr" << ',' << "psr"
//        << ',' << "femalePC" << ',' << "malePC" << ',' << "preference"
//        << ',' << "ornaments" << ',' << "popSize" << ',' << "mateMale"
//        << ',' << "mateFemale" << ',' << "careMale" << ',' << "careFemale"
//        << ',' << "recoMale" << ',' << "recoFemale" << ',' << "juvMale" << ',' << "juvFemale" << std::endl;
//
//      offspringTrait << "day" << ',' << "female" << ',' << "malep1" << ',' << "malet" << ',' << "malepc1" << ',' << "malepc2" << ','
//        << "femalep1" << ',' << "femalet" << ',' << "femalepc1" << ',' << "femalepc2" << ','
//        << "offp1" << ',' << "offt" << ',' << "offpc1" << ',' << "offpc2" << ',' << "offjuvdays" << std::endl;
//      deadIndividual << "day" << ',' << "individual" << ',' << ',' << "preference" << ',' << "ornaments" << ',' << "pc1" << ',' << "pc2" << ',' << "matingT"
//        << ',' << "numOff" << ',' << "numSurOff" << std::endl;
//
//
//      for (int day = 0; day < tEnd; ++day)
//      {
//        if (day % 1000 == 0)
//        {
//          double osr = osrValue();
//          double asr = asrValue();
//          double psr = psrValue();
//          allTrait << day << ',' << osr << ',' << asr << ',' << psr;
//        }
//
//        // males and females seek mating opportunities and produce offspring
//        getCaringDays(day);
//
//        // Individuals switch to pre-mating state after caring 
//        gotoRecover(day);
//
//        // Individuals re-enter the mating state after pre-mating
//        backtoMating(day);
//
//        // offspring would enter the mating state after getting mature
//        juvToMating();
//
//        if (day % 1000 == 0) {
//          std::cout << day << ':' << "mating pool" << "\t" << mateMale.size() << '\t' << mateFemale.size() << std::endl;
//          std::cout << day << ':' << "caringing pool" << '\t' << careMale.size() << '\t' << careFemale.size() << std::endl;
//          std::cout << day << ':' << "recoveringing pool" << '\t' << recoMale.size() << '\t' << recoFemale.size() << std::endl;
//          std::cout << day << ':' << "juving pool" << '\t' << juvMale.size() << '\t' << juvFemale.size() << std::endl;
//
//          int mateMalePop = static_cast <int> (mateMale.size());
//          int mateFemalePop = static_cast <int> (mateFemale.size());
//          int careMalePop = static_cast <int> (careMale.size());
//          int careFemalePop = static_cast <int> (careFemale.size());
//          int recoMalePop = static_cast <int> (recoMale.size());
//          int recoFemalePop = static_cast <int> (recoFemale.size());
//          int juvMalePop = static_cast <int> (juvMale.size());
//          int juvFemalePop = static_cast <int> (juvFemale.size());
//          int popSize = static_cast <int> (mateMale.size()) + static_cast <int> (mateFemale.size())
//            + static_cast <int> (careMale.size()) + static_cast <int> (careFemale.size())
//            + static_cast <int> (recoMale.size()) + static_cast <int> (recoFemale.size())
//            + static_cast <int> (juvMale.size()) + static_cast <int> (juvFemale.size());
//
//          averageTrait();
//          allTrait << ',' << popSize << ',' << mateMalePop << ',' << mateFemalePop << ',' <<
//            careMalePop << ',' << careFemalePop << ',' << recoMalePop << ',' << recoFemalePop << ',' <<
//            juvMalePop << ',' << juvFemalePop << std::endl;
//
//        }
//
//      }
//      allTrait.close();
//      offspringTrait.close();
//      para.close();
//      deadIndividual.close();
//    }
//  }
//}
//
#include <filesystem>
#include "evorole.h"
#include "simulation.h"
#include "recorder.h"


namespace evorole {

  void Parameter::sanity_check(const Parameter& param)
  {
    // ToDo: test test test, throw if something seems fishy
  }


  void run_simulation(const Parameter& param, const std::filesystem::path& out, bool verbose)
  {
    auto recorder = std::make_unique<Recorder>(param, out, verbose);
    Simulation(param, std::move(recorder)).run();
  }

}
