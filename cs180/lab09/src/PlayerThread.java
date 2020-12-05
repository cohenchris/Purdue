/**
 * Note: You do not need to change this file
 */
public class PlayerThread implements Runnable {
    private Player player;
    private Action action;
 
    public PlayerThread(Player player, Action action) {
        this.player = player;
        this.action = action;
    }
 
    @Override
    public void run() {
    	switch(action){
    	case DECX:
	        for (int i = 0; i < 10000; i++) {
	            player.moveLeft();
	        }
	        break;
    	case INCX:
	        for (int i = 0; i < 10000; i++) {
	            player.moveRight();
	        }
	        break;
    	case DECY:
	        for (int i = 0; i < 10000; i++) {
	            player.moveUp();
	        }
	        break;
    	case INCY:
	        for (int i = 0; i < 10000; i++) {
	            player.moveDown();
	        }
	        break;
    	case DECHP:
	        for (int i = 0; i < 10000; i++) {
	            player.loseHealth();;
	        }
	        break;
    	case INCHP:
	        for (int i = 0; i < 10000; i++) {
	            player.gainHealth();
	        }
	        break;
	        
    	}
    }
}