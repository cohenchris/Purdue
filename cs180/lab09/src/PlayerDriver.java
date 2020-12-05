/**
 * Note: You do not need to change this file
 */
public class PlayerDriver {
	public static void main(String[] args) throws InterruptedException{
		Player player = new Player(0, 0, 5000);
		Thread decX = new Thread(new PlayerThread(player, Action.DECX));
		Thread incX = new Thread(new PlayerThread(player, Action.INCX));
		Thread decY = new Thread(new PlayerThread(player, Action.DECY));
		Thread incY = new Thread(new PlayerThread(player, Action.INCY));
		Thread decHP = new Thread(new PlayerThread(player, Action.DECHP));
		Thread incHP = new Thread(new PlayerThread(player, Action.INCHP));
		decX.start();
		incX.start();
		decY.start();
		incY.start();
		decHP.start();
		incHP.start();
		
		decX.join();
		incX.join();
		decY.join();
		incY.join();
		decHP.join();
		incHP.join();
		player.printPlayer();
	}
}
