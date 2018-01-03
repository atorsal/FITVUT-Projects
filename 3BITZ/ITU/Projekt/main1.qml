import QtQuick 2.3
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import timetracker 1.0


ColumnLayout{

        QMLTimeTracker{
            id:tracker
        }
        anchors.fill: parent

        spacing: 0


      Button{
            id: button1
            background: Rectangle {
                //#b7ceb3,#d6d6d6" : "#F4EADE,#6699CC
                //color: button1.down ? "#d6d6d6" : "#303030"
                id:rec1
                color:button1.hovered ? "#d6d6d6" : "#303030"
                border.color: "#191919"
                //#26282a
                border.width: 1
                radius: 4

                }


                Image {
                    anchors.margins: 5
                    anchors.fill: parent
                    source: ":/images/home7.png"
                    fillMode: Image.PreserveAspectFit
                    anchors.bottomMargin:20
                }
                Text {
                    id:text1
                    text: "Home"
                    //color: button1.down ? "#000000" : "#dfe2db"
                    color:button1.hovered ? "#000000" : "#dfe2db"
                    anchors.bottom: parent.bottom // Placing text in bottom
                    anchors.horizontalCenter: parent.horizontalCenter // Centering text
                    renderType: Text.NativeRendering // Rendering type (optional)
                    font.bold:true
                    font.pointSize:15
                }

                onClicked:{

                   tracker.test(0);
                   // if(button1.checked){
                    rec1.color= "#d6d6d6";
                    text1.color ="#000000";
                    rec2.color= "#303030";
                    text2.color ="#dfe2db";
                    rec3.color= "#303030";
                    text3.color ="#dfe2db";
                    rec4.color= "#303030";
                    text4.color ="#dfe2db";
                    rec5.color= "#303030";
                    text5.color ="#dfe2db";
                   // }
                }
               /* onFocusChanged: {
                    rec1.color= "#303030";
                    text1.color ="#dfe2db";
                }*/
                    //rec1.color= button1.focus ? "#d6d6d6" : "#303030";
                    //text1.color ="#dfe2db";
                    //button1.focus = false;


                checkable: true
                //focus: true
                hoverEnabled: true
                Layout.fillWidth: true
                Layout.fillHeight: true
    }

    Button{
       id: button2
       background: Rectangle {
           id:rec2
           //color: button2.down ? "#d6d6d6" : "#303030"
           color:button2.hovered ? "#d6d6d6" : "#303030"
           border.color: "#191919"
           border.width: 1
           radius: 4
       }
       Image {
           anchors.margins: 5
           anchors.fill: parent
           source: ":/images/edit_6.png"
           fillMode: Image.PreserveAspectFit
           anchors.bottomMargin:30
       }
       Text {
           id:text2
           //color:button2.down ? "#000000" : "#dfe2db"
           color:button2.hovered ? "#000000" : "#dfe2db"
           text: "TODO list"
           anchors.bottom: parent.bottom // Placing text in bottom
           anchors.horizontalCenter: parent.horizontalCenter // Centering text
           renderType: Text.NativeRendering // Rendering type (optional)
           font.bold:true
           font.pointSize:15
       }

       onClicked:{

          tracker.test(1);
          // if(button2.checked){
          rec2.color= "#d6d6d6";
          text2.color ="#000000";

           rec1.color= "#303030";
           text1.color ="#dfe2db";
           rec3.color= "#303030";
           text3.color ="#dfe2db";
           rec4.color= "#303030";
           text4.color ="#dfe2db";
           rec5.color= "#303030";
           text5.color ="#dfe2db";

          // }
       }
       /*onFocusChanged: {
           rec2.color= "#303030";
           text2.color ="#dfe2db";
       }*/

        checkable: true
        //focus:true
        hoverEnabled:true
        Layout.fillWidth: true
        Layout.fillHeight: true
    }
    Button{
        id: button4

        background: Rectangle {
            id:rec3
            //color: button4.down ? "#d6d6d6" : "#303030"
            color:button4.hovered ? "#d6d6d6" : "#303030"
            border.color: "#191919"
            border.width: 1
           radius: 4
        }

        Image {
            anchors.margins: 5
            anchors.fill: parent
            source: ":/images/folder5.png"
            fillMode: Image.PreserveAspectFit
            anchors.bottomMargin:25
        }
        Text {
           id:text3
           text: "Categories"
          // color:button4.down ? "#000000" : "#dfe2db"
           color:button4.hovered ? "#000000" : "#dfe2db"
           anchors.margins: 2
           anchors.bottom: parent.bottom
           anchors.horizontalCenter: parent.horizontalCenter
           renderType: Text.NativeRendering

           font.bold:true
           font.pointSize:15
        }
        onClicked:{

           tracker.test(2);
           // if(button4.checked){
            rec3.color= "#d6d6d6";
            text3.color ="#000000";

            rec2.color= "#303030";
            text2.color ="#dfe2db";
            rec1.color= "#303030";
            text1.color ="#dfe2db";
            rec4.color= "#303030";
            text4.color ="#dfe2db";
            rec5.color= "#303030";
            text5.color ="#dfe2db";

           // }
        }
        /*onFocusChanged: {
            rec3.color= "#303030";
            text3.color ="#dfe2db";
        }*/
        checkable: true
        //focus:true
        hoverEnabled:true
        Layout.fillWidth: true
        Layout.fillHeight: true
   }
    Button{
       id: button3
       background: Rectangle {
       //#b7ceb3
           id:rec4
           //color: button3.down ? "#d6d6d6" : "#303030"
           color:button3.hovered ? "#d6d6d6" : "#303030"
           border.color: "#191919"
           border.width: 1
           radius: 4
       }
       Image{
              anchors.margins: 5
              anchors.fill: parent
              source: ":/images/bar_chart7.png"
              fillMode: Image.PreserveAspectFit
              anchors.bottomMargin:30

       }
       Text {
             id:text4
             text: "Statistics"
             //color: button3.down ? "#000000" : "#dfe2db"
             color: button3.hovered ? "#000000" : "#dfe2db"
             anchors.bottom: parent.bottom // Placing text in bottom
             anchors.horizontalCenter: parent.horizontalCenter // Centering text
             renderType: Text.NativeRendering // Rendering type (optional)
             font.bold:true
             font.pointSize:15
       }
       onClicked:{

          tracker.test(3);
          // if(button3.checked){
           rec4.color= "#d6d6d6";
           text4.color ="#000000";

           rec2.color= "#303030";
           text2.color ="#dfe2db";
           rec3.color= "#303030";
           text3.color ="#dfe2db";
           rec1.color= "#303030";
           text1.color ="#dfe2db";
           rec5.color= "#303030";
           text5.color ="#dfe2db";

          // }
       }
       /*onFocusChanged: {

                rec4.color= "#303030";
                text4.color ="#dfe2db";


       }*/
       checkable: true
       //focus:true
       hoverEnabled:true
       Layout.fillWidth: true
       Layout.fillHeight: true

     }

    Button{
        id: button5
        background: Rectangle {
            id: rec5
            //color: button5.down ? "#d6d6d6" : "#303030"
            color: button5.hovered ? "#d6d6d6" : "#303030"
            border.color: "#191919"
            border.width: 1
            radius: 4

        }
        Image {
            anchors.margins: 5
            anchors.fill: parent
            source: ":/images/settings7.png"
            fillMode: Image.PreserveAspectFit
            anchors.bottomMargin:30
        }
        Text {
           id:text5
           text: "Settings"
           //color: button5.down ? "#000000" : "#dfe2db"
           color: button5.hovered ? "#000000" : "#dfe2db"
           anchors.margins: 2
           anchors.bottom: parent.bottom // Placing text in bottom
           anchors.horizontalCenter: parent.horizontalCenter // Centering text
           renderType: Text.NativeRendering // Rendering type (optional)
           font.bold:true
           font.pointSize:15
        }
        onClicked:{

           tracker.test(4);
            //if(button5.checked){
            rec5.color= "#d6d6d6";
            text5.color ="#000000";

            rec2.color= "#303030";
            text2.color ="#dfe2db";
            rec3.color= "#303030";
            text3.color ="#dfe2db";
            rec4.color= "#303030";
            text4.color ="#dfe2db";
            rec1.color= "#303030";
            text1.color ="#dfe2db";
            //}

        }

        /*onFocusChanged: {
            rec5.color= "#303030";
            text5.color ="#dfe2db";
        }*/
        checkable: true
        hoverEnabled:true
        Layout.fillWidth: true
        Layout.fillHeight: true
   }


}
