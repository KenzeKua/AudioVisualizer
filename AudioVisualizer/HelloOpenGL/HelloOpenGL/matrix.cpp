#include "matrix.h"

//! Identity matrix.
float Matrix::sIdentity[4][4] =
{
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
};

void Matrix::invertMatrixGeneral(const float *m, float *out)
{
	/* NB. OpenGL Matrices are COLUMN major. */
	#define MAT(m,r,c) (m)[(c)*4+(r)]

	/* Here's some shorthand converting standard (row,column) to index. */
	#define m11 MAT(m,0,0)
	#define m12 MAT(m,0,1)
	#define m13 MAT(m,0,2)
	#define m14 MAT(m,0,3)
	#define m21 MAT(m,1,0)
	#define m22 MAT(m,1,1)
	#define m23 MAT(m,1,2)
	#define m24 MAT(m,1,3)
	#define m31 MAT(m,2,0)
	#define m32 MAT(m,2,1)
	#define m33 MAT(m,2,2)
	#define m34 MAT(m,2,3)
	#define m41 MAT(m,3,0)
	#define m42 MAT(m,3,1)
	#define m43 MAT(m,3,2)
	#define m44 MAT(m,3,3)

	float det;
	float d12, d13, d23, d24, d34, d41;
	float tmp[16]; /* Allow out == in. */

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	/* pre-compute 2x2 dets for last two rows when computing */
	/* cofactors of first two rows. */
	d12 = (m31*m42-m41*m32);
	d13 = (m31*m43-m41*m33);
	d23 = (m32*m43-m42*m33);
	d24 = (m32*m44-m42*m34);
	d34 = (m33*m44-m43*m34);
	d41 = (m34*m41-m44*m31);

	tmp[0] =  (m22 * d34 - m23 * d24 + m24 * d23);
	tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
	tmp[2] =  (m21 * d24 + m22 * d41 + m24 * d12);
	tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

	/* Compute determinant as early as possible using these cofactors. */
	det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];

	/* Run singularity test. */
	if (det == 0.0) {
		/* printf("invert_matrix: Warning: Singular matrix.\n"); */
		memcpy(out,sIdentity,16*sizeof(float));
	}
	else {
		float invDet = 1.0f / det;
		/* Compute rest of inverse. */
		tmp[0] *= invDet;
		tmp[1] *= invDet;
		tmp[2] *= invDet;
		tmp[3] *= invDet;

		tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
		tmp[5] =  (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
		tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
		tmp[7] =  (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = m11*m22-m21*m12;
		d13 = m11*m23-m21*m13;
		d23 = m12*m23-m22*m13;
		d24 = m12*m24-m22*m14;
		d34 = m13*m24-m23*m14;
		d41 = m14*m21-m24*m11;

		tmp[8] =  (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
		tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
		tmp[10] =  (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
		tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
		tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
		tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
		tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
		tmp[15] =  (m31 * d23 - m32 * d13 + m33 * d12) * invDet;

		memcpy(out, tmp, 16*sizeof(float));
	}

	#undef m11
	#undef m12
	#undef m13
	#undef m14
	#undef m21
	#undef m22
	#undef m23
	#undef m24
	#undef m31
	#undef m32
	#undef m33
	#undef m34
	#undef m41
	#undef m42
	#undef m43
	#undef m44
	#undef MAT
}


//
// Invert matrix m.  This algorithm contributed by Stephane Rehel
// <rehel@worldnet.fr>
//

void Matrix::invertMatrix(const float *m, float *out)
{
	/* NB. OpenGL Matrices are COLUMN major. */
	#define MAT(m,r,c) (m)[(c)*4+(r)]

	/* Here's some shorthand converting standard (row,column) to index. */
	#define m11 MAT(m,0,0)
	#define m12 MAT(m,0,1)
	#define m13 MAT(m,0,2)
	#define m14 MAT(m,0,3)
	#define m21 MAT(m,1,0)
	#define m22 MAT(m,1,1)
	#define m23 MAT(m,1,2)
	#define m24 MAT(m,1,3)
	#define m31 MAT(m,2,0)
	#define m32 MAT(m,2,1)
	#define m33 MAT(m,2,2)
	#define m34 MAT(m,2,3)
	#define m41 MAT(m,3,0)
	#define m42 MAT(m,3,1)
	#define m43 MAT(m,3,2)
	#define m44 MAT(m,3,3)

	register float det;
	float tmp[16]; /* Allow out == in. */

	if( m41 != 0. || m42 != 0. || m43 != 0. || m44 != 1. ) {
		invertMatrixGeneral(m, out);
		return;
	}

	/* Inverse = adjoint / det. (See linear algebra texts.)*/

	tmp[0]= m22 * m33 - m23 * m32;
	tmp[1]= m23 * m31 - m21 * m33;
	tmp[2]= m21 * m32 - m22 * m31;

	/* Compute determinant as early as possible using these cofactors. */
	det= m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2];

	/* Run singularity test. */
	if (det == 0.0) {
		/* printf("invert_matrix: Warning: Singular matrix.\n"); */
		memcpy( out, sIdentity, 16*sizeof(float) );
	}
	else {
		float d12, d13, d23, d24, d34, d41;
		register float im11, im12, im13, im14;

		det= 1.0f / det;

		/* Compute rest of inverse. */
		tmp[0] *= det;
		tmp[1] *= det;
		tmp[2] *= det;
		tmp[3]  = 0.0f;

		im11= m11 * det;
		im12= m12 * det;
		im13= m13 * det;
		im14= m14 * det;
		tmp[4] = im13 * m32 - im12 * m33;
		tmp[5] = im11 * m33 - im13 * m31;
		tmp[6] = im12 * m31 - im11 * m32;
		tmp[7] = 0.0f;

		/* Pre-compute 2x2 dets for first two rows when computing */
		/* cofactors of last two rows. */
		d12 = im11*m22 - m21*im12;
		d13 = im11*m23 - m21*im13;
		d23 = im12*m23 - m22*im13;
		d24 = im12*m24 - m22*im14;
		d34 = im13*m24 - m23*im14;
		d41 = im14*m21 - m24*im11;

		tmp[8] =  d23;
		tmp[9] = -d13;
		tmp[10] = d12;
		tmp[11] = 0.0f;

		tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23);
		tmp[13] =  (m31 * d34 + m33 * d41 + m34 * d13);
		tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12);
		tmp[15] =  1.0f;

		memcpy(out, tmp, 16*sizeof(float));
	}

	#undef m11
	#undef m12
	#undef m13
	#undef m14
	#undef m21
	#undef m22
	#undef m23
	#undef m24
	#undef m31
	#undef m32
	#undef m33
	#undef m34
	#undef m41
	#undef m42
	#undef m43
	#undef m44
	#undef MAT
}
