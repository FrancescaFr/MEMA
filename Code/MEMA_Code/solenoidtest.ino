int on = 0;
void setup() {
  // put your setup code here, to run once:
pinMode (7, OUTPUT);
pinMode (13, OUTPUT);
pinMode (2, INPUT);


}

void loop() {
 digitalWrite (7, HIGH);
 on = digitalRead(2);
if (on ==HIGH)
{
digitalWrite(13, HIGH);
delay(50);
digitalWrite(13,LOW);
}
else
{ digitalWrite(13,LOW);
}
on = 0;
delay(6000);
  // put your main code here, to run repeatedly:

}
