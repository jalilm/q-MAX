#ifndef QMAX_UTILS_HPP
#define QMAX_UTILS_HPP

#include <cstring>

using namespace std;

enum class TEST {
  VE, FE, HH, Timing, STiming, PrioritySampling
};
enum class DATASET {
  CAIDA, CAIDA18, UCLA, UCLA_FULL, UNIV1, UNIV2, RANDOM
};

inline string testName(const TEST &t) {
  switch (t) {
    case TEST::VE:return "ve";
    case TEST::FE:return "fe";
    case TEST::HH:return "hh";
    case TEST::Timing:return "timing";
    case TEST::PrioritySampling:return "ps";
    case TEST::STiming:return "stiming";
  }
}

inline string datasetName(const DATASET &d) {
  switch (d) {
    case DATASET::CAIDA:return "caida";
    case DATASET::CAIDA18:return "caida18";
    case DATASET::UCLA:return "ucla";
    case DATASET::UCLA_FULL:return "ucla_full";
    case DATASET::UNIV1:return "univ1";
    case DATASET::UNIV2:return "univ2";
    case DATASET::RANDOM:return "random";
  }
}

inline TEST testEnum(const char *c) {
  if (strcmp(c, "VE") == 0) {
    return TEST::VE;
  } else if (strcmp(c, "FE") == 0) {
    return TEST::FE;
  } else if (strcmp(c, "HH") == 0) {
    return TEST::HH;
  } else if (strcmp(c, "timing") == 0) {
    return TEST::Timing;
  } else if (strcmp(c, "ps") == 0) {
    return TEST::PrioritySampling;
  }
  throw invalid_argument("Please provide a supported test: VE, FE, HH, WVE, WFE, WHH.");
}

inline DATASET datasetEnum(const char *c) {
  if (strcmp(c, "CAIDA") == 0) {
    return DATASET::CAIDA;
  } else if (strcmp(c, "UCLA") == 0) {
    return DATASET::UCLA;
  } else if (strcmp(c, "UCLA_FULL") == 0) {
    return DATASET::UCLA_FULL;
  } else if (strcmp(c, "UNIV1") == 0) {
    return DATASET::UNIV1;
  } else if (strcmp(c, "UNIV2") == 0) {
    return DATASET::UNIV2;
  } else if (strcmp(c, "RANDOM") == 0) {
    return DATASET::RANDOM;
  } else if (strcmp(c, "CAIDA18") == 0) {
    return DATASET::CAIDA18;
  }
  throw invalid_argument("Please provide a supported dataset: CAIDA, CAIDA18, UCLA, UCLA_FULL, UNIV1, UNIV2.");
}

inline string getFrequencyLimit(DATASET dataset) {
  switch (dataset) {
    case DATASET::CAIDA:
    case DATASET::UCLA:return "30000000";
    case DATASET::UCLA_FULL:return "99000000";
    case DATASET::UNIV1:return "18000000";
    case DATASET::UNIV2:throw invalid_argument("UNIV2 not yet supported for frequency estimation (FE).");
    case DATASET::RANDOM:return "1000000";
  }
}

#endif //QMAX_UTILS_HPP
