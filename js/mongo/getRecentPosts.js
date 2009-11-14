function (limit) {

    var limit = limit || 10;

    var r = [];

    var a = db.post.find( { }, { _id:1, title:1 } )
            .sort( { date: -1 } )
            .limit(limit);

    return a.toArray();
}
