#include <iostream>
#include <string>
#include <chrono>
#include <opencv2\opencv.hpp>
#include <opencv2\core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\videoio.hpp>


extern "C" {
#include "vc.h"
}


void vc_timer(void) {
	static bool running = false;
	static std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();

	if (!running) {
		running = true;
	}
	else {
		std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::duration elapsedTime = currentTime - previousTime;

		// Tempo em segundos.
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(elapsedTime);
		double nseconds = time_span.count();

		std::cout << "Tempo decorrido: " << nseconds << "segundos" << std::endl;
		std::cout << "Pressione qualquer tecla para continuar...\n";
		std::cin.get();
	}
}


int main(void) {
	// Vídeo
	char videofile[20] = "video1.mp4";
	cv::VideoCapture capture;
	
	struct
	{
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;
	// Outros
	std::string str;
	int key = 0;
	OVC* nobjetos;
	OVC* nobjetos2;
	int numero[3];
	int numero2[3];
	IVC* image[11];
	IVC* image2;
	IVC* imagel;
	IVC** letras;
	IVC* letrasC[12];
	IVC* letrasH[6];
	int* coluna;
	int* linha;
	char caracter[6];
	
	
	

	/* Leitura de vídeo de um ficheiro */
	/* NOTA IMPORTANTE:
	O ficheiro video.avi deverá estar localizado no mesmo directório que o ficheiro de código fonte.
	*/
	capture.open(videofile);

	/* Em alternativa, abrir captura de vídeo pela Webcam #0 */
	//capture.open(0, cv::CAP_DSHOW); // Pode-se utilizar apenas capture.open(0);

	/* Verifica se foi possível abrir o ficheiro de vídeo */
	if (!capture.isOpened())
	{
		std::cerr << "Erro ao abrir o ficheiro de vídeo!\n";
		return 1;
	}

	/* Número total de frames no vídeo */
	video.ntotalframes = (int)capture.get(cv::CAP_PROP_FRAME_COUNT);
	/* Frame rate do vídeo */
	video.fps = (int)capture.get(cv::CAP_PROP_FPS);
	/* Resolução do vídeo */
	video.width = (int)capture.get(cv::CAP_PROP_FRAME_WIDTH);
	video.height = (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	/* Cria uma janela para exibir o vídeo */
	cv::namedWindow("VC - VIDEO", cv::WINDOW_AUTOSIZE);

	/* Inicia o timer */
	vc_timer();

	cv::Mat frame;
	while (key != 'q') {
		/* Leitura de uma frame do vídeo */
		capture.read(frame);

		/* Verifica se conseguiu ler a frame */
		if (frame.empty()) break;

		/* Número da frame a processar */
		video.nframe = (int)capture.get(cv::CAP_PROP_POS_FRAMES);

		/* Exemplo de inserção texto na frame */
		str = std::string("RESOLUCAO: ").append(std::to_string(video.width)).append("x").append(std::to_string(video.height));
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 25), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("TOTAL DE FRAMES: ").append(std::to_string(video.ntotalframes));
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 50), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("FRAME RATE: ").append(std::to_string(video.fps));
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 75), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		str = std::string("N. DA FRAME: ").append(std::to_string(video.nframe));
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 100), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);
		const char* str_caracters = caracter;
		str = std::string("MATRICULA: ").append(str_caracters, 6);
		cv::putText(frame, str, cv::Point(20, 125), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 0, 0), 2);
		cv::putText(frame, str, cv::Point(20, 125), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(255, 255, 255), 1);


		// Faça o seu código aqui...

#pragma region criar imagens
		// Cria uma nova imagem IVC
		image[0] = vc_image_new(video.width, video.height, 3, 255);
		// Copia dados de imagem da estrutura cv::Mat para uma estrutura IVC
		memcpy(image[0]->data, frame.data, video.width * video.height * 3);

		image[1] = vc_image_new(video.width, video.height, 3, 255);
		memcpy(image[1]->data, frame.data, video.width * video.height * 3);

		image[2] = vc_image_new(video.width, video.height, 1, 255);

		image[3] = vc_image_new(video.width, video.height, 1, 255);

		image[4] = vc_image_new(video.width, video.height, 1, 255);

		image[5] = vc_image_new(video.width, video.height, 1, 255);

#pragma endregion

		//transformar rgb para hsv
		vc_rgb_to_hsv(image[1]);

		//segmentar para encontrar os blobs
		vc_hsv_segmentation(image[1], 0, 360, 0, 100, 55, 100);
		
		vc_rgb_to_gray(image[1], image[2]);

		//erode-dilate
	    vc_binary_open(image[2], image[3], 2, 7);
		

		//dilate-erode
		vc_binary_close(image[3], image[4], 7, 2);
		

		//dar lable aos blobs
		nobjetos = vc_binary_blob_labelling(image[4], image[5], &numero[0]);
	
		//informaçao
		vc_binary_blob_info(image[5], nobjetos, numero[0]);

		
		image2 = vc_draw_image(nobjetos, image[0], numero[0]);
		
		if (image2 != NULL) 
		{
			
			imagel = vc_image_new(image2->width, image2->height, 3, image2->levels);
			vc_copy(image2, imagel);
			image[6] = vc_image_new(image2->width, image2->height, 1, image2->levels);

			image[7] = vc_image_new(image2->width, image2->height, 1, image2->levels);

			image[8] = vc_image_new(image2->width, image2->height, 1, image2->levels);

			image[9] = vc_image_new(image2->width, image2->height, 1, image2->levels);
			
			//transformar rgb para hsv
			vc_rgb_to_hsv(image2);

			//vc_write_image((char*)"test1.ppm", image2);

			//segmentar para encontrar os blobs
			vc_hsv_segmentation(image2, 0, 360, 0, 100, 0, 45);
			//vc_write_image((char*)"test2.ppm", image2);

			vc_rgb_to_gray(image2, image[6]);

			//erode-dilate
			vc_binary_open(image[6], image[7], 1, 2);
			//vc_write_image((char*)"test3.ppm", image[6]);

			//dilate-erode
			vc_binary_close(image[7], image[8], 1, 1);
			//vc_write_image((char*)"test4.ppm", image[7]);

			//dar lable aos blobs
			nobjetos2 = vc_binary_blob_labelling(image[8], image[9], &numero2[0]);
			/*vc_write_image((char*)"test5.ppm", image[8]);
			vc_write_image((char*)"test6.ppm", image[9]);*/

			//informaçao
			vc_binary_blob_info(image[9], nobjetos2, numero2[0]);

			//desenhar os quadrados nos blob
			if (vc_draw_imageL(nobjetos2, imagel, numero2[0]) != NULL) {
				letras = vc_draw_imageL(nobjetos2, imagel, numero2[0]);

				//criar as imagens cinza
				letrasC[0] = vc_image_new(letras[0]->width, letras[0]->height, 1, letras[0]->levels);
				letrasC[1] = vc_image_new(letras[1]->width, letras[1]->height, 1, letras[1]->levels);
				letrasC[2] = vc_image_new(letras[2]->width, letras[2]->height, 1, letras[2]->levels);
				letrasC[3] = vc_image_new(letras[3]->width, letras[3]->height, 1, letras[3]->levels);
				letrasC[4] = vc_image_new(letras[4]->width, letras[4]->height, 1, letras[4]->levels);
				letrasC[5] = vc_image_new(letras[5]->width, letras[5]->height, 1, letras[5]->levels);


				//Passar as letras para cinzento
				vc_rgb_to_gray(letras[0], letrasC[0]);
				vc_rgb_to_gray(letras[1], letrasC[1]);
				vc_rgb_to_gray(letras[2], letrasC[2]);
				vc_rgb_to_gray(letras[3], letrasC[3]);
				vc_rgb_to_gray(letras[4], letrasC[4]);
				vc_rgb_to_gray(letras[5], letrasC[5]);

				//Passar as letras para binario
				vc_gray_to_binary_global_mean(letrasC[0]);
				vc_gray_to_binary_global_mean(letrasC[1]);
				vc_gray_to_binary_global_mean(letrasC[2]);
				vc_gray_to_binary_global_mean(letrasC[3]);
				vc_gray_to_binary_global_mean(letrasC[4]);
				vc_gray_to_binary_global_mean(letrasC[5]);
				vc_write_image((char*)"1.ppm", letrasC[0]);
				/*vc_write_image((char*)"2.ppm", letrasC[1]);
				vc_write_image((char*)"3.ppm", letrasC[2]);
				vc_write_image((char*)"4.ppm", letrasC[3]);
				vc_write_image((char*)"5.ppm", letrasC[4]);
				vc_write_image((char*)"6.ppm", letrasC[5]);*/


				

				//ciclo para detectar os carateres 
				for (int i = 0; i <= 5; i++)
				{
					int picoH = 0;
					int picoV = 0;
					int baixaH = 0;
					int baixaV = 0;
					int mediaH = 0;
					int mediaV = 0;

					linha = conta_linha(letrasC[i]);
					coluna = conta_coluna(letrasC[i]);

					//ciclo para percorrer a altura toda para saber os resultados das linhas
					for (int x = 0; x < letrasC[i]->height; x++)
					{
						float oheh = ((float)linha[x] / (float)letrasC[i]->width);
						//comparar a quantidade de pixeis pretos com a largura
						if (oheh > 0.9)picoH++;
						if (oheh < 0.3)baixaH++;
						if (oheh < 0.6 && oheh  > 0.4)mediaH++;

					}

					//ciclo para percorrer a largura toda para saber os resultados das colunas
					for (int p = 0; p < letrasC[i]->width; p++)
					{
						float bouas = ((float)coluna[p] / (float)letrasC[i]->height);
						//comparar a quantidade de pixeis pretos com a altura
						if (bouas > 0.9)picoV++;
						if (bouas < 0.3)baixaV++;
						if ((bouas < 0.6) && (bouas > 0.4))mediaV++;

					}


					//para letra 2
					if (picoH >= 0 && picoH <= 8 && picoV >= 0 && picoV <= 1 && baixaH >= 1 && baixaH <= 14 && baixaV >= 0 && baixaV <= 11 && mediaH <= 32 && mediaH >= 5 && mediaV <= 38 && mediaV >= 5)
					{

						caracter[i] = (char)'2';

					}

					//para o numero 7
					if (picoH >= 8 && picoH <= 11 && picoV >= 7 && picoV <= 10 && baixaH >= 28 && baixaH <= 35 && baixaV >= 10 && baixaV <= 17 && mediaH <= 2 && mediaV <= 2) 
					{
						
							caracter[i] = (char)'7';
						
					}

					//para a letras Q
					if (picoH >= 2 && picoH <= 8 && picoV >= 0 && picoV <= 7 && baixaH >= 0 && baixaH <= 4 && baixaV >= 0 && baixaV <= 3 && mediaH <= 16 && mediaH >= 11 && mediaV <= 27 && mediaV >= 23)
					{

						caracter[i] = (char)'Q';

					}

					//para a letras R
					if (picoH >= 5 && picoH <= 11 && picoV >= 6 && picoV <= 13 && baixaH >= 0 && baixaH <= 6 && baixaV >= 0 && baixaV <= 2 && mediaH <= 25 && mediaV <= 25 && mediaV >= 20)
					{

						caracter[i] = (char)'R';

					}

					//para a letras 9
					if (picoH >= 0 && picoH <= 4 && picoV >= 0 && picoV <= 2 && baixaH >= 6 && baixaH <= 11 && baixaV >= 0 && baixaV <= 3 && mediaH <= 19 && mediaH >= 14 && mediaV <= 13 && mediaV >= 8)
					{

						caracter[i] = (char)'9';

					}

					//para a letras 8
					if (picoH >= 0 && picoH <= 2 && picoV >= 0 && picoV <= 2 && baixaH >= 0 && baixaH <= 3 && baixaV >= 0 && baixaV <= 2 && mediaH <= 30 && mediaH >= 21 && mediaV <= 17 && mediaV >= 9)
					{

						caracter[i] = (char)'8';

					}

					//para letra 6
					if (picoH >= 0 && picoH <= 2 && picoV >= 0 && picoV <= 2 && baixaH >= 3 && baixaH <= 8 && baixaV >= 0 && baixaV <= 3 && mediaH <= 29 && mediaH >= 23 && mediaV <= 17 && mediaV >= 11)
					{

						caracter[i] = (char)'6';

					}

					

					//para letra 6
					if (picoH >= 7 && picoH <= 9 && picoV >= 0 && picoV <= 1 && baixaH >= 10 && baixaH <= 13 && baixaV >= 10 && baixaV <= 13 && mediaH <= 6 && mediaH >= 5 && mediaV <= 8 && mediaV >= 5)
					{

						caracter[i] = (char)'6';

					}

				

					//para letra U
					if (picoH >= 0 && picoH <= 2 && picoV >= 0 && picoV <= 1 && baixaH >= 1 && baixaH <= 3 && baixaV >= 0 && baixaV <= 2 && mediaH <= 36 && mediaH >= 19 && mediaV <= 39 && mediaV >= 34)
					{

						caracter[i] = (char)'U';

					}

				

					

					//printf("picoH:%d\n", picoH);
					//printf("picoV:%d\n", picoV);
					//printf("baixaH:%d\n", baixaH);
					//printf("baixaV:%d\n", baixaV);
					//printf("mediaH:%d\n", mediaH);
					//printf("mediaV:%d\n\n\n", mediaV);
				}


				
			
				//vc_write_image((char*)"letra0.ppm", letras[0]);
				//vc_write_image((char*)"letra1.ppm", letras[1]);
				//vc_write_image((char*)"letra2.ppm", letras[2]);
				//vc_write_image((char*)"letra3.ppm", letras[3]);
				//vc_write_image((char*)"letra4.ppm", letras[4]);
				//vc_write_image((char*)"letra5.ppm", letras[5]);

				//vc_write_image((char*)"letra6.ppm", letrasC[0]);
				//vc_write_image((char*)"letra7.ppm", letrasC[1]);
				//vc_write_image((char*)"letra8.ppm", letrasC[2]);
				//vc_write_image((char*)"letra9.ppm", letrasC[3]);
				//vc_write_image((char*)"letra10.ppm", letrasC[4]);
				//vc_write_image((char*)"letra11.ppm", letrasC[5]);
				
			}
			

			

			
			
		}

		// Copia dados de imagem da estrutura IVC para uma estrutura cv::Mat
		//memcpy(frame.data, image->data, video.width * video.height * 3);

		// Liberta a memória da imagem IVC que havia sido criada
		
		
		
		memcpy(frame.data, image[0]->data, video.width * video.height * 3);
		


		// +++++++++++++++++++++++++

		/* Exibe a frame */
		cv::imshow("VC - VIDEO", frame);

		/* Sai da aplicação, se o utilizador premir a tecla 'q' */
		key = cv::waitKey(1);
	}
	//vc_write_image("tb1.pgm", image[0]);
	

			

	//vc_write_image((char*)"tb2.pgm", image[1]);
	//system("FilterGear tb2.pgm"); // Output
	//vc_image_free(image[1]);

	//vc_write_image((char*)"tb3.ppm", image[2]);
	//system("FilterGear tb3.ppm"); // Output
	//vc_image_free(image[2]);

	//vc_write_image((char*)"tb4.ppm", image[3]);
	//system("FilterGear tb4.ppm"); // Output
	//vc_image_free(image[3]);

	//vc_write_image((char*)"tb5.ppm", image[4]);
	//system("FilterGear tb5.ppm"); // Output
	//vc_image_free(image[4]);

	//vc_write_image((char*)"tb6.ppm", image[5]);
	//system("FilterGear tb6.ppm"); // Output
	//vc_image_free(image[5]);

	//vc_write_image((char*)"tb7.ppm", image[6]);
	//system("FilterGear tb7.ppm"); // Output
	//vc_image_free(image[6]);

	//vc_write_image((char*)"tb8.ppm", image[7]);
	//system("FilterGear tb8.ppm"); // Output
	//vc_image_free(image[7]);

	//vc_write_image((char*)"tb9.ppm", image[8]);
	//system("FilterGear tb9.ppm"); // Output
	//vc_image_free(image[8]);

	//vc_write_image((char*)"tb1.ppm", image[0]);
	//system("FilterGear tb1.ppm"); // Output
	//vc_image_free(image[0]);

	
	/* Para o timer e exibe o tempo decorrido */
	vc_timer();

	/* Fecha a janela */
	cv::destroyWindow("VC - VIDEO");

	/* Fecha o ficheiro de vídeo */
	capture.release();

	return 0;
}