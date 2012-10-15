// Fix magnetic field ... so that we can display
// old data from alien.

void mf_fix(Double_t f=-1.)
{
  IlcMagF* fld = new IlcMagF("Map", "Map", f, 1.0, IlcMagF::k5kG, IlcMagF::kNoBeamField, -1.0);
  TGeoGlobalMagField::Instance()->SetField(fld);
  TGeoGlobalMagField::Instance()->Lock();
}
