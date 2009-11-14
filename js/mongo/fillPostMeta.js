function (p) {
    p.tags = [];
    p.cats = [];

    db.tag.find({ _id: { $in: p._tags } }).forEach(function (z) {
        p.tags.push(z);
    });

    db.cat.find({ _id: { $in: p._cats } }).forEach(function (z) {
        p.cats.push(z);
    });
}
