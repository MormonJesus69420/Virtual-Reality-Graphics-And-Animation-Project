var com;

function createComponent(){
    com = Qt.createComponent("point.qml");
}

function createPoint(x, y){
    return com.createObject(root, {"x": x, "y": y});
}
