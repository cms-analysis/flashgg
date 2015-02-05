const int nchann = 32;
const char *chann[nchann] = { "comb", "comb_HPA", "comb_HPA_clean", //3
                              "hbb", "htt", "hgg", "hww", "hzz",    //5
                              "ggH", "qqH", "VH", "ttH", // 4
                              "VH_hbb", "ttH_hbb",  "ggH_hgg", "qqH_hgg", "ggH_hww", "qqH_hww", "VH_hww", "ggH_htt", "qqH_htt", "VH_htt", // 10
                              "hzz4l",
                              "lowres", "hires", "fermions", "bosons", // 4
                              "hgg0", "hires0", "hzz2", "hires2", "comb2"
                         };
const char *CHANN[nchann] = { "COMB",   "HPA",      "HPA_CLEAN", 
                              "HBB",    "HTT", "HGG", "HWW",  "HZZ",
                              "GGH", "QQH", "VH", "TTH", 
                              "VH_HBB", "TTH_HBB",  "GGH_HGG", "QQH_HGG", "GGH_HWW", "QQH_HWW", "VH_HWW", "GGH_HTT", "QQH_HTT", "VH_HTT",
                              "HZZ4L",
                              "LOWRES", "HIRES", "FERMIONS", "BOSONS",
                              "HGG0", "HIRES0", "HZZ2", "HIRES2", "COMB2"
                             };

const int ne = 3;
const char *epre[ne] = { "", "7_", "8_" };
const char *epst[ne] = { "", "_7", "_8" };

