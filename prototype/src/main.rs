// Design
//
mod luna {
    pub struct Model<'a> {
        pub w: &'a mut [f32],
        pub b: &'a mut [f32],
    }

    impl<'a> Model<'a> {
        pub fn new(blob: &'a mut [f32]) -> Model<'a> {
            let (left, right) = blob.split_at_mut(5);
            Model { w: left, b: right }
        }
    }

    pub fn add(o: &mut [f32], a: &[f32], b: &[f32]) {
        let s = std::cmp::min(std::cmp::min(a.len(), b.len()), o.len());
        for i in 0..s {
            o[i] = a[i] + b[i];
        }
    }
}

fn main() {
    let mut blob = [0f32; 10];
    let mut m = luna::Model::new(&mut blob);
    let mut actv = [0f32; 10];
    luna::add(&mut actv, m.w, m.b);
    luna::add(&mut m.w, &actv, m.b);
    luna::add(&mut m.b, &actv, m.w);
}
