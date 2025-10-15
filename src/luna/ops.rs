pub fn op_matmul(m: usize, k: usize, n: usize, o: &mut [f32], a: &[f32], b: &[f32]) {
    assert!(a.len() == m * k);
    assert!(b.len() == k * n);
    assert!(o.len() == m * n);
    for row in 0..m {
        for col in 0..n {
            let mut v = 0f32;
            for x in 0..k {
                v += a[row * k + x] * b[x * n + col];
            }
            o[row * n + col] = v;
        }
    }
}
#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_op_matmul() {
        let a = [1f32, 2f32];
        let b = [3f32, 4f32];
        let mut o = [0f32];
        op_matmul(1, 2, 1, &mut o, &a, &b);
        assert_eq!(o[0], 11f32);
    }
}
