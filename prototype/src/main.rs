// Design
//

mod luna {
    pub struct TensorView<'a> {
        pub shape: Vec<isize>,
        pub data: &'a mut [f32],
    }

    pub struct Model<'a> {
        pub w: TensorView<'a>,
        pub b: TensorView<'a>,
    }

    impl<'a> Model<'a> {
        pub fn new(blob: &'a mut [f32]) -> Model<'a> {
            let (left, right) = blob.split_at_mut(5);
            Model {
                w: TensorView {
                    shape: vec![5],
                    data: left,
                },
                b: TensorView {
                    shape: vec![5],
                    data: right,
                },
            }
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
    assert_eq!(m.w.shape, m.b.shape);
    luna::add(&mut actv, m.w.data, m.b.data);
    luna::add(&mut m.w.data, &actv, m.b.data);
    luna::add(&mut m.b.data, &actv, m.w.data);
}
