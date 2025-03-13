//---------------------------
// QCD Scale factors (global)
//---------------------------
Double_t myHist::getSF_QCD_global(){
  if(_campaign == "2018_UL"){
    if     (_channel == "mm") return 0.12970339;
    else if(_channel == "me") return 0.07222484;
    else if(_channel == "em") return 0.19266066;
    else if(_channel == "ee") return 0.1;
    else return 0.1;
  }
  else if(_campaign == "2017_UL"){
    if     (_channel == "mm") return 0.11850897;
    else if(_channel == "me") return 0.08928256;
    else if(_channel == "em") return 0.03900507;
    else if(_channel == "ee") return 0.1;
    else return 0.1;
  }
  else if(_campaign == "2016postVFP_UL"){
    if     (_channel == "mm") return 0.10749282;
    else if(_channel == "me") return 0.08240349;
    else if(_channel == "em") return 0.04435222;
    else if(_channel == "ee") return 0.1;
    else return 0.1;
  }
  else if(_campaign == "2016preVFP_UL"){
    if     (_channel == "mm") return 0.11457822;
    else if(_channel == "me") return 0.16575052;
    else if(_channel == "em") return 0.04367684;
    else if(_channel == "ee") return 0.1;
    else return 0.1;
  }
  else{
    cout<<"Warning: QCD SF not found!"<<endl;
    return 1.0;
  }
} 
//---------------------------------
// TTBar Scale factors (HT-binned)
//---------------------------------
Double_t myHist::getSF_TTBar_HTbinned(float ht) {
  if (_campaign == "2018_UL") {
    if (_channel == "mm") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "me") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "em") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "ee") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    return 1.0;
  }
  if (_campaign == "2017_UL") {
    if (_channel == "mm") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "me") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "em") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "ee") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    return 1.0;
  }
  if (_campaign == "2016postVFP_UL") {
    if (_channel == "mm") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "me") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "em") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "ee") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    return 1.0;
  }
  if (_campaign == "2016preVFP_UL") {
    if (_channel == "mm") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "me") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "em") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    else if (_channel == "ee") {
      if (ht < 50)               return 1.0;
      if (50 < ht && ht <= 100)  return 1.0;
      if (100 < ht && ht <= 200) return 1.0;
      if (200 < ht && ht <= 400) return 1.0;
      if (400 < ht)              return 1.0;
    }
    return 1.0;
  }
  return 1.0;
}
