import processing.serial.*;

int[][] dataArray = new int[3][600];
int[] dataArrayval = new int[3];
int[] dataArrayvalmin = new int[3];
int[] dataArrayvalmax = new int[3];
color[] dataArraycolor = {
  color(224, 24, 106),
  color(48, 105, 166),
  color(85, 130, 48)};

void mousePressed() {
  //reset min and max
  for(int a=0; a<dataArrayvalmin.length; a++) {
    dataArrayvalmin[a] = Integer.MIN_VALUE;
  }
  for(int a=0; a<dataArrayvalmax.length; a++) {
    dataArrayvalmax[a] = Integer.MAX_VALUE;
  }
}

Serial myPort;
void setup() {  
  size(600, 400);
  myPort = new Serial(this, "COM42",  9600);
  //center all variables
  for(int a=0; a<dataArrayval.length; a++) {
    for (int b=0; b<dataArray[a].length; b++) {   
      dataArray[a][b] = height/2;
    }
  }
  //reset min and max
  for(int a=0; a<dataArrayvalmin.length; a++) {
    dataArrayvalmin[a] = 0;
  }
  for(int a=0; a<dataArrayvalmax.length; a++) {
    dataArrayvalmax[a] = 4096;
  }
}

void draw()
{ 
  background(255); // white
  
  //draw paper
  for (int i=0; i<=width/10; i++) {      
    stroke(200);
    line((-frameCount%10)+i*10, 0, (-frameCount%10)+i*10, height);
    line(0, i*10, width, i*10);
  }
  
  //draw indication lines (90deg, 180deg, 270deg)
  stroke(0);
  for (int i=1; i<=3; i++)
    line(0, height/4*i, width, height/4*i);

  //write last data
  PFont f = createFont("Arial", 16, true);
  textFont(f,16);
  for(int a=0; a<dataArrayval.length; a++) {
    if(dataArrayval[a] > dataArrayvalmin[a])
      dataArrayvalmin[a] = dataArrayval[a];
    if(dataArrayval[a] < dataArrayvalmax[a])
      dataArrayvalmax[a] = dataArrayval[a];
    fill(dataArraycolor[a]);
    text(dataArrayval[a] + " (" + dataArrayvalmin[a] + " , " + dataArrayvalmax[a] + ")", 20, 20 + a*21);    
  } 
 
  //add data
  for(int a=0; a<dataArrayval.length; a++) {
    dataArray[a][dataArray[a].length-1] = (int)map(4096-dataArrayval[a], 0, 4096, 0, height);
  }
  
  //draw line
  for(int a=0; a<dataArrayval.length; a++) {
    noFill();
    stroke(dataArraycolor[a]);
    beginShape();
    for (int b=0; b<dataArray[a].length; b++) { 
      vertex(b, dataArray[a][b]); 
    }
    endShape();
  }
  
  //put all data one array back
  for(int a=0; a<dataArrayval.length; a++) {
    for (int b=1; b<dataArray[a].length; b++) { 
      dataArray[a][b-1] = dataArray[a][b];
    }
  }
}

void serialEvent (Serial arduino) {
  byte[] buf = new byte[128];
  int bufread = myPort.readBytesUntil((byte)10, buf);

  if(bufread == 7) {
    dataArrayval[0] = (short)((buf[1] & 0xff)<<8 | (buf[0] & 0xff));
    dataArrayval[1] = (short)((buf[3] & 0xff)<<8 | (buf[2] & 0xff));
    dataArrayval[2] = (short)((buf[5] & 0xff)<<8 | (buf[4] & 0xff));
    //print(dataArrayval[0]); print(" ");
    //print(dataArrayval[1]); print(" ");
    //print(dataArrayval[2]); print(" ");
    //println("");
  }
}

