function (limit) {

    var limit = limit || 10;

    var r = [];

    var a = db.post.find( { } )
            .sort( { date: -1 } )
            .limit(limit);

    for (var i = 0, x; x = a[i]; ++i) {
        //fillPostMeta(x);
        //delete x._tags;
        //delete x._cats;

        //x.numComs = x.coms.length;
        delete x.coms;

        r.push(x);
    }
    return r;
}
