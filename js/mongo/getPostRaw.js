function (id) {

    var p = db.post.findOne( { _id: id } );

    if (p) {
        fillPostMeta(p);
        //delete p._tags;
        //delete p._cats;
        p.numComs = p.coms.length;
        return p;
    }

    return null; // caller checks isNull()
}
