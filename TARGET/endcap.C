#include <TMath.h>
#include <TString.h>
#include <stdio.h>

void endcap() {

int layer;
int nladders;
float gradi_rot;
float z_ladder;
float radius;
float dimx1,dimx2,dimy1,dimy2,dimz;
float radiant;
float x_ladder,y_ladder,z_ladder;
float rot;
float RMin,RMax;
char rad[5];
layer=0;
nladders=12;
gradi_rot=360/nladders;
RMin=1.6;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderA\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);

printf("<trd name=\"VertexEndcap_detA\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz-0.0001);


for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderA\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detA_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=1;
nladders=12;
gradi_rot=360/nladders;
RMin=1.6;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderB\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);


for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderB\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detA_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=2;
nladders=12;
gradi_rot=360/nladders;
RMin=2.0;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderC\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);

printf("<trd name=\"VertexEndcap_detB\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz-0.0001);


for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderC\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detB_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=3;
nladders=12;
gradi_rot=360/nladders;
RMin=2.0;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderD\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);

for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderD\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detB_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=4;
nladders=12;
gradi_rot=360/nladders;
RMin=1.6;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderE\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);

for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderE\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detA_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer-4);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=5;
nladders=12;
gradi_rot=360/nladders;
RMin=1.6;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderF\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);

for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderF\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detA_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer-4);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=6;
nladders=12;
gradi_rot=360/nladders;
RMin=2.0;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderG\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);


for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderG\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detB_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer-4);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");

layer=7;
nladders=12;
gradi_rot=360/nladders;
RMin=2.0;
RMax=7.5;
radiant=(2*TMath::Pi())/nladders;
dimz=RMax*TMath::Cos(15.*TMath::DegToRad())-RMin*TMath::Cos(15.*TMath::DegToRad());
dimx1=2.*RMax*TMath::Sin(15.*TMath::DegToRad());
dimx2=2.*RMin*TMath::Sin(15.*TMath::DegToRad());
dimy1=dimy2=0.0113;
radius=RMin*TMath::Cos(15.*TMath::DegToRad())+(dimz/2.);
x_ladder=0.0;
y_ladder=0.0;
z_ladder=0.0;
rot=0.0;

printf("<trd name=\"VertexEndcap_ladderH\" x1=\"%f\" x2=\"%f\" y1=\"%f\" y2=\"%f\" z=\"%f\" />\n",dimx1,dimx2,dimy1,dimy2,dimz);

for (int ladder=0;ladder<nladders;ladder++) {
	printf("      <volume name=\"VertexEndcap_layer%d_ladder%d%s_volume\">\n",layer,ladder,rad);
        printf("        <materialref ref=\"TARGET_Silicon\" />\n");
        printf("        <solidref ref=\"VertexEndcap_ladderH\" />\n");
        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_detB_volume\" />\n");
        printf("          <positionref ref=\"identity_pos\" />\n");
        printf("          <rotationref ref=\"identity_rot\" />\n");
        printf("        </physvol>\n");
	printf("      </volume>\n");
}

printf("      <volume name=\"VertexEndcap_layer%d_volume\">\n",layer);
printf("        <materialref ref=\"TARGET_Air\" />\n");
printf("        <solidref ref=\"VertexEndcap_layer%d\" />\n",layer-4);

for (ladder=0;ladder<nladders;ladder++) {
	x_ladder=radius*(TMath::Cos((TMath::Pi()/2)-(ladder*radiant)));
	y_ladder=radius*(TMath::Sin((TMath::Pi()/2)-(ladder*radiant)));

        printf("        <physvol>\n");
        printf("          <volumeref ref=\"VertexEndcap_layer%d_ladder%d%s_volume\" />\n",layer,ladder,rad);
        printf("          <position name=\"posEnd_lay%d_lad%d\" x=\"%f\" y=\"%f\" z=\"%f\" unit=\"mm\" />\n",layer,ladder,x_ladder,y_ladder,z_ladder);
        printf("          <rotation name=\"rotEnd_lay%d_lad%d\" x=\"-90.0\" y=\"%f\" z=\"0.0\" unit=\"radian\" />\n",layer,ladder,rot);
        printf("          <positionref ref=\"posEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("          <rotationref ref=\"rotEnd_lay%d_lad%d\" />\n",layer,ladder);
        printf("        </physvol>\n");

        rot=rot+gradi_rot;
}
printf("      </volume>\n");
}
