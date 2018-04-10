/************************************************
Michael McLaughlin
Comp Arch
Professor Menendez
Project 2 - Learn
Learn is a program that reads in data from two text files regarding the pricing of houses.  It
uses matrices and linear algebra operations to solve a system of equations and predict what the
cost of a house will most likely be based on the given data.
************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Function declarations
double **createMatrix(int, int);
void printMatrix(double**, int, int);
void transposeMatrix(double**, double**, int, int);
void multiplyMatrices(int, int, double**, int, int, double**, double**);
double **findInverse(double **matrix, int size);

int main(int argc, char **argv)
{
	//Checking to make sure the correct arguments were passed, the files exist, and the
	//files are not empty.
	if (argc != 3) //Making sure right number of files were passed
	{
		printf("error\n");
		return 0;
	}
	FILE *trainFilePoint;
	trainFilePoint = fopen(argv[1], "r");
	FILE *dataFilePoint;
	dataFilePoint = fopen(argv[2], "r");
	if (trainFilePoint == NULL || dataFilePoint == NULL) //Making sure files exist
	{
		printf("error\n");
		return 0;
	}
	int nextTrainChar, nextDataChar;
	nextTrainChar = fgetc(trainFilePoint);
	nextDataChar = fgetc(dataFilePoint);
	if (nextTrainChar == EOF || nextDataChar == EOF) //Making sure files are not empty
	{
		printf("error\n");
		return 0;
	}
	rewind(trainFilePoint);
	rewind(dataFilePoint);
	//Initial checks ended.  Begin operations on train file
	char fileType[20];
	fscanf(trainFilePoint, "%s", fileType);
	//Making sure that the files were passed in the correct order: train first, data second
	if (strcmp(fileType, "train") == 1) {printf("error\n"); return 0;}
	int rows, columns, i , j;
	fscanf(trainFilePoint, "%d", &columns);
	fscanf(trainFilePoint, "%d", &rows);
	columns++; //Incrementing for 1's column
	int sizeCheck = columns; //Saving column size to check later that the matrices match
	double **trainMatrix = createMatrix(rows, columns);
	double **priceVector = createMatrix(rows, 1);
	double **trainTranspose = createMatrix(columns, rows);
	double **multipliedMatrix = createMatrix(columns, columns);
	for (i = 0; i < rows; i++) //Populating the training matrix and price vector
	{
		for (j = 0; j < columns; j++)
		{
			if (j == 0)
			{
				trainMatrix[i][j] = 1.0;
				continue;
			}
			fscanf(trainFilePoint, "%lf", &trainMatrix[i][j]);
		}
		fscanf(trainFilePoint, "%lf", &priceVector[i][0]);
	}
	//Linear Algebra magic
	transposeMatrix(trainMatrix, trainTranspose, rows, columns);
	multiplyMatrices(columns, rows, trainTranspose, rows, columns, trainMatrix, multipliedMatrix);
	double **inverse = findInverse(multipliedMatrix, columns);
	double **weightVector = createMatrix(columns, 1);
	double **temp = createMatrix(columns, rows);
	multiplyMatrices(columns, columns, inverse, columns, rows, trainTranspose, temp);
	multiplyMatrices(columns, rows, temp, rows, 1, priceVector, weightVector);
	//Weight vector acquired.  Beginning operations on data file
	fscanf(dataFilePoint, "%s", fileType);
	//Check to make sure that the second file passed is the correct type
	if (strcmp(fileType, "data") == 1) {printf("error\n"); return 0;}
	fscanf(dataFilePoint, "%d", &columns);
	fscanf(dataFilePoint, "%d", &rows);
	columns++; //Incrementing for 1's column
	//Checking that the train and data matrices are of matching sizes
	if (columns != sizeCheck) {printf("error\n"); return 0;}
	double **dataMatrix = createMatrix(rows, columns);
	double **outputPrices = createMatrix(rows, 1);
	for (i = 0; i < rows; i++) //Populating data matrix
	{
		for (j = 0; j < columns; j++)
		{
			if (j == 0)
			{
				dataMatrix[i][j] = 1.0;
				continue;
			}
			fscanf(dataFilePoint, "%lf", &dataMatrix[i][j]);
		}
	}
	//CS_Student uses Linear Algebra!  It's super effective!
	multiplyMatrices(rows, columns, dataMatrix, columns, 1, weightVector, outputPrices);
	//Final prices acquired.  Printing result
	printMatrix(outputPrices, rows, 1);
	//Freeing memory
	free(trainMatrix);
	free(priceVector);
	free(trainTranspose);
	free(multipliedMatrix);
	free(inverse);
	free(weightVector);
	free(temp);
	free(dataMatrix);
	free(outputPrices);
	return 0;
}

/*
**Function: createMatrix()
**Accepts: Two integer arguments representing the row and column sizes of the matrix that is to be created.
**Returns: A double pointer of type double.
**Notes: This function uses malloc to dynamically create an MxN matrix of type double, based on the dimensions
**passed in.  Checks to make sure that the memory was properly allocated.
*/
double **createMatrix(int rows, int columns)
{
	int i;
	double **matrix = (double**)malloc(rows * sizeof(double*));
	if (matrix == NULL) {printf("error\n"); exit(0);} //Check if memory was allocated
	for (i = 0; i < rows; i++)
	{
		matrix[i] = (double*)malloc(columns * sizeof(double));
		if (matrix[i] == NULL) {printf("error\n"); exit(0);} //Check if memory was allocated
	}
	return matrix;
}

/*
**Function: printMatrix()
**Accepts: A double pointer of type double for the matrix, and two integers for the dimensions of the matrix.
**Returns: Nothing.
**Notes:  Uses the matrix and dimensions to loop through the matrix and print out the formatted values.
*/
void printMatrix(double **matrix, int rows, int columns)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < columns; j++)
		{
			printf("%.0f\n", matrix[i][j]);
		}
	}
}

/*
**Function: transposeMatrix()
**Accepts: A double pointer of type double for the 1st matrix, a double pointer to put the transposed matrix in
**to, and two integers for the dimensions of the matrix.
**Returns: Nothing.
**Notes: Transposes the first matrix that is passed and stores it in the second.  I.E. the rows of matrix 1
**are now the columns of matrix 2, and the columns of matrix 1 are now the rows of matrix 2.  The dimensions
**of matrix 1 are passed in order to structure the loop.
*/
void transposeMatrix(double **matrix, double **transposed, int rows, int columns)
{
	int i, j;
	for (i = 0; i < columns; i++)
	{
		for (j = 0; j < rows; j++)
		{
			transposed[i][j] = matrix[j][i];
		}
	}
}

/*
**Function: multiplyMatrices()
**Accepts: Two integers for the rows and columns of matrix 1, followed by a double pointer of type double for
**the 1stmatrix, followed by two integers for the rows and columns of matrix 2, followed by a double pointer of
**type double for the 2nd matrix, followed by another double pointer of type double to store the resulting
**product.
**Returns: Nothing.
**Notes: The arguments being passed can be thought of as "An MxN matrix times an NxM matrix will give us this
**product."  Multiplies two matrices.  Nothing fancy.
*/
void multiplyMatrices(int row1, int column1, double **matrix1,
		      int row2, int column2, double **matrix2, double **product)
{
	int i, j, k;
	for (i = 0; i < row1; i++)
	{
		for (j = 0; j < column2; j++)
		{
			for (k = 0; k < column1; k++)
			{
				product[i][j] += matrix1[i][k] * matrix2[k][j];
			}
		}
	}
}

/*
**Function: findInverse()
**Accepts: A (double**) for the matrix that is to be inverted, and an (int) for the size of the matrix.
**Returns: A (double**) for the inverted matrix.
**Notes: findInverse, AKA The Big Daddy™, takes in a matrix, creates an ID matrix that is (theoretically, not
**actually) augmented on to the passed matrix, then performs elementary row operations to find the inverse.
*/
double **findInverse(double **matrix, int size)
{
	double **augmented = createMatrix(size, size);
	int i, j, k, l;
	double tempNum = 0;
	//Populating the identity matrix
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (i == j) {augmented[i][j] = 1;}
			else {augmented[i][j] = 0;}
		}
	}
	//Beginning row reduction in order to get our matrix into an upper triangular form.  Four different
	//counters were needed to run the loops: i and j to iterate through the rows and columns, k to iterate
	//across a row for a row operation, and l to iterate down a column when searching for zeros underneath
	//a given pivot point.
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			//Every row operation uses a pivot as the starting point.  This allows for greater
			//flow and control during the reduction of the matrix.
			if (i == j)
			{
				if (matrix[i][j] != 1) //Scaling a row if a given pivot is not 1
				{
					tempNum = matrix[i][j];
					for (k = 0; k < size; k++) //Iterates across row to be scaled
					{
						matrix[i][k] /= tempNum;
						augmented[i][k] /= tempNum;
					}
				}
				//Iterates down a column once a given pivot is set to 1.  l needs to be
				//set to 1, so that the pivot itself is not set to zero upon the check.  Starts
				//from the location underneath each pivot, and iterates to the end of that
				//column.
				for (l = 1; l < size - i; l++)
				{
					if (matrix[i + l][j] != 0) //Check if location is already 0
					{
						tempNum = matrix[i + l][j];
						//Iterates through row, performing required operations
						for (k = 0; k < size; k++)
						{
							matrix[i + l][k] -= (tempNum * matrix[i][k]);
							augmented[i + l][k] -= (tempNum * augmented[i][k]);
						}
					}
				}
			}
		}
	}
	//Matrix is now upper triangular.  Now, in reverse:
	for (i = size - 1; i > -1; i--)
	{
		for (j = size - 1; j > -1; j--)
		{
			if (i == j)
			{
				for (l = 1; l < (i + 1); l++)
				{
					if (matrix[i - l][j] != 0)
					{
						tempNum = matrix[i - l][j];
						for (k = size - 1; k > -1; k--)
						{
							matrix[i - l][k] -= (tempNum * matrix[i][k]);
							augmented[i - l][k] -= (tempNum * augmented[i][k]);
						}
					}
				}
			}
		}
	}
	//Sweet.
	return augmented;
}









